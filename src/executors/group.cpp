#include "global.h"

/**
 * @brief File contains method to process GROUP commands.
 *
 * syntax:
 * new_table> <- GROUP BY <grouping_attribute> FROM <table_name> HAVING
<aggregate(attribute)> <bin_op> <attribute_value> RETURN
<aggregate_func(attribute)>
 */
bool syntacticParseGROUP()
{
    logger.log("syntacticParseGroup");
    if (tokenizedQuery.size() != 13 && tokenizedQuery[3] != "BY" && tokenizedQuery[5] != "FROM" && tokenizedQuery[7] != "HAVING" && tokenizedQuery[11] != "RETURN")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = GROUP;
    parsedQuery.groupRelationName = tokenizedQuery[6];
    parsedQuery.groupColumnName = tokenizedQuery[4];
    parsedQuery.groupResultRelationName = tokenizedQuery[0];
    parsedQuery.groupAttributeValue = tokenizedQuery[10];
    string binaryOperator = tokenizedQuery[9];
    if (binaryOperator == "<")
        parsedQuery.groupBinaryOperator = LESS_THAN;
    else if (binaryOperator == ">")
        parsedQuery.groupBinaryOperator = GREATER_THAN;
    else if (binaryOperator == ">=" || binaryOperator == "=>")
        parsedQuery.groupBinaryOperator = GEQ;
    else if (binaryOperator == "<=" || binaryOperator == "=<")
        parsedQuery.groupBinaryOperator = LEQ;
    else if (binaryOperator == "==")
        parsedQuery.groupBinaryOperator = EQUAL;
    else if (binaryOperator == "!=")
        parsedQuery.groupBinaryOperator = NOT_EQUAL;
    else
    {
        cout << "SYNTAC ERROR" << endl;
        return false;
    }
    string temp = tokenizedQuery[8];
    cout << temp << endl;
    string aggregatePattern = R"((MAX|MIN|COUNT|SUM|AVG)\((\w+)\))";
    regex pattern(aggregatePattern);
    smatch match;
    if (regex_match(temp, match, pattern))
    {
        parsedQuery.groupFunction = match[1];
        parsedQuery.groupReturnColumnName = match[2];
    }
    else
    {
        cout << "SYNTAC ERROR: Aggregate function error" << endl;
        return false;
    }
    temp = tokenizedQuery[12];
    if (regex_match(temp, match, pattern) && match[2] == parsedQuery.groupReturnColumnName)
    {
        parsedQuery.groupReturnFunction = match[1];
    }
    else
    {
        cout << "SYNTAC ERROR: Return attribute doesn't match aggregating attribute" << endl;
        return false;
    }
    return true;
}

bool semanticParseGROUP()
{
    logger.log("semanticParseGROUP");
    if (tableCatalogue.isTable(parsedQuery.groupResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }
    if (!tableCatalogue.isTable(parsedQuery.groupRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    if (!tableCatalogue.isColumnFromTable(parsedQuery.groupColumnName, parsedQuery.groupRelationName) || !tableCatalogue.isColumnFromTable(parsedQuery.groupReturnColumnName, parsedQuery.groupRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }

    return true;
}

void executeGROUP()
{
    logger.log("executeGROUP");
    Table table = *tableCatalogue.getTable(parsedQuery.groupRelationName);
    Table *sortedTable = new Table("SortedTable", table.columns);
    Cursor cursor = table.getCursor();
    vector<int> row = cursor.getNext();
    while (!row.empty())
    {
        sortedTable->writeRow<int>(row);
        row = cursor.getNext();
    }
    cout << "SORTING\n" << endl;
    if (sortedTable->blockify())
    {
        tableCatalogue.insertTable(sortedTable);
        parsedQuery.sortRelationName = "SortedTable";
        parsedQuery.sortColumnName.clear();
        parsedQuery.sortingStrategy.clear();
        parsedQuery.sortColumnName.push_back(parsedQuery.groupColumnName);
        parsedQuery.sortingStrategy.push_back("ASC");
        executeSORT(false);
        int columnIndex = sortedTable->getColumnIndex(parsedQuery.groupColumnName);
        cursor = sortedTable->getCursor();
        row = cursor.getNext();
        int prev = -DEFAULT;
        TableCatalogue groups;
        cout << row[0]<< row[1]<< "REACHED\n";
        while (!row.empty())
        {
            Table *group = new Table(to_string(row[columnIndex]), sortedTable->columns);
            cout<<"prev "<<prev<<endl;
            while (row[columnIndex] == prev)
            {
                row = cursor.getNext();
                if (row.empty())
                    break;
                group->writeRow<int>(row);
            }
            if (group->blockify())
            {
                groups.insertTable(group);
            }
            else
            {
                group->unload();
                delete group;
            }
            prev = row[columnIndex];
        }
        // select groups based on having condition
        cout<<"gals"<<endl;
        vector<string> retCols;
        retCols.push_back(parsedQuery.groupColumnName);
        retCols.push_back(parsedQuery.groupReturnFunction + parsedQuery.groupReturnColumnName);
        Table *resultantTable = new Table(parsedQuery.groupResultRelationName, retCols);
        int sum = 0, maximum = -DEFAULT, minimum = DEFAULT, count = 0;
        cout<<"guys"<<endl;
        for (auto iterat : groups.tables)
        {
            
            cout<< iterat.first << endl;
            // int groupID = stoi(iterat.first);
            // Table group = *iterat.second;
            // cursor = group.getCursor();
            // while (!row.empty())
            // {
            //     row = cursor.getNext();
            //     if (row.empty())
            //         break;
            //     sum += row[group.getColumnIndex(parsedQuery.groupReturnColumnName)];
            //     maximum = max(row[group.getColumnIndex(parsedQuery.groupReturnColumnName)], maximum);
            //     minimum = min(row[group.getColumnIndex(parsedQuery.groupReturnColumnName)], minimum);
            //     count++;
            // }
            // vector<int> retRow;
            // retRow.push_back(groupID);
            // if (parsedQuery.groupReturnFunction == "SUM")
            //     retRow.push_back(sum);
            // else if (parsedQuery.groupReturnFunction == "MAX")
            //     retRow.push_back(maximum);
            // else if (parsedQuery.groupReturnFunction == "MIN")
            //     retRow.push_back(minimum);
            // else if (parsedQuery.groupReturnFunction == "COUNT")
            //     retRow.push_back(count);
            // else if (parsedQuery.groupReturnFunction == "AVG")
            //     retRow.push_back(floor(sum / count));
            // if (evaluateBinOp(retRow[1], stoi(parsedQuery.groupAttributeValue), parsedQuery.groupBinaryOperator))
            //     resultantTable->writeRow<int>(retRow);
        }
        if (resultantTable->blockify())
        {
            tableCatalogue.insertTable(resultantTable);
            cout << "GROUP EXECUTED SUCCESSFULLY\n";
        }
        else
        {
            cout << "Empty Table" << endl;
            resultantTable->unload();
            delete resultantTable;
        }
    }
    else
    {
        cout << "Empty Table" << endl;
        sortedTable->unload();
        delete sortedTable;
    }

    return;
}