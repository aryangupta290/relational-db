#include "global.h"
/**
 * @brief
 * SYNTAX: R <- JOIN relation_name1, relation_name2 ON column_name1 bin_op column_name2
 */
bool syntacticParseJOIN()
{
    logger.log("syntacticParseJOIN");
    if (tokenizedQuery.size() != 9 || tokenizedQuery[5] != "ON")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = JOIN;
    parsedQuery.joinResultRelationName = tokenizedQuery[0];
    parsedQuery.joinFirstRelationName = tokenizedQuery[3];
    parsedQuery.joinSecondRelationName = tokenizedQuery[4];
    parsedQuery.joinFirstColumnName = tokenizedQuery[6];
    parsedQuery.joinSecondColumnName = tokenizedQuery[8];

    string binaryOperator = tokenizedQuery[7];
    if (binaryOperator == "<")
        parsedQuery.joinBinaryOperator = LESS_THAN;
    else if (binaryOperator == ">")
        parsedQuery.joinBinaryOperator = GREATER_THAN;
    else if (binaryOperator == ">=" || binaryOperator == "=>")
        parsedQuery.joinBinaryOperator = GEQ;
    else if (binaryOperator == "<=" || binaryOperator == "=<")
        parsedQuery.joinBinaryOperator = LEQ;
    else if (binaryOperator == "==")
        parsedQuery.joinBinaryOperator = EQUAL;
    else if (binaryOperator == "!=")
        parsedQuery.joinBinaryOperator = NOT_EQUAL;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    return true;
}

bool semanticParseJOIN()
{
    logger.log("semanticParseJOIN");

    if (tableCatalogue.isTable(parsedQuery.joinResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.joinFirstRelationName) || !tableCatalogue.isTable(parsedQuery.joinSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.joinFirstColumnName, parsedQuery.joinFirstRelationName) || !tableCatalogue.isColumnFromTable(parsedQuery.joinSecondColumnName, parsedQuery.joinSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    return true;
}

void executeJOIN()
{
    logger.log("executeJOIN");
    Table *firstTable = tableCatalogue.getTable(parsedQuery.joinFirstRelationName);
    Table *secondTable = tableCatalogue.getTable(parsedQuery.joinSecondRelationName);
    // copy these tables to new tables and sort them
    Table *firstTableCopy = new Table(parsedQuery.joinFirstRelationName + "_copy", firstTable->columns);
    Table *secondTableCopy = new Table(parsedQuery.joinSecondRelationName + "_copy", secondTable->columns);
    Cursor firstCursor = firstTable->getCursor();
    vector<int> firstRow = firstCursor.getNext();
    while (!firstRow.empty())
    {
        firstTableCopy->writeRow<int>(firstRow);
        firstRow = firstCursor.getNext();
    }
    Cursor secondCursor = secondTable->getCursor();
    vector<int> secondRow = secondCursor.getNext();
    while (!secondRow.empty())
    {
        secondTableCopy->writeRow<int>(secondRow);
        secondRow = secondCursor.getNext();
    }
    if (!firstTableCopy->blockify() || !secondTableCopy->blockify())
    {
        cout << "ERROR: Failed to blockify tables" << endl;
        return;
    }
    // sorting copies
    tableCatalogue.insertTable(firstTableCopy);
    parsedQuery.sortRelationName = firstTableCopy->tableName;
    parsedQuery.sortColumnName.clear();
    parsedQuery.sortColumnName.push_back(parsedQuery.joinFirstColumnName);
    parsedQuery.sortingStrategy.clear();
    parsedQuery.sortingStrategy.push_back("ASC");
    executeSORT(false);

    tableCatalogue.insertTable(secondTableCopy);
    parsedQuery.sortRelationName = secondTableCopy->tableName;
    parsedQuery.sortColumnName.clear();
    parsedQuery.sortColumnName.push_back(parsedQuery.joinSecondColumnName);
    parsedQuery.sortingStrategy.clear();
    parsedQuery.sortingStrategy.push_back("ASC");
    executeSORT(false);

    // joining sorted tables
    auto header = firstTable->columns;
    header.insert(header.end(), secondTable->columns.begin(), secondTable->columns.end());
    Table *joinResult = new Table(parsedQuery.joinResultRelationName, header);

    firstCursor = firstTableCopy->getCursor();
    auto firstCursor2 = firstTableCopy->getCursor();
    auto firstRow2 = firstCursor2.getNext();
    secondCursor = secondTableCopy->getCursor();
    firstRow = firstCursor.getNext();
    secondRow = secondCursor.getNext();
    vector<int> joinRow, firstValue, secondValue;
    int firstJoinColumn = firstTable->getColumnIndex(parsedQuery.joinFirstColumnName);
    int secondJoinColumn = secondTable->getColumnIndex(parsedQuery.joinSecondColumnName);
    switch (parsedQuery.joinBinaryOperator)
    {
    case EQUAL:
    {
        while (!firstRow.empty() && !secondRow.empty())
        {
            if (firstRow[firstJoinColumn] < secondRow[secondJoinColumn])
            {
                firstRow = firstCursor.getNext();
                firstRow2 = firstCursor2.getNext();
            }
            else if (firstRow[firstJoinColumn] > secondRow[secondJoinColumn])
            {
                secondRow = secondCursor.getNext();
            }
            else
            {
                firstValue = firstRow;
                secondValue = secondRow;
                joinRow = firstRow;
                joinRow.insert(joinRow.end(), secondRow.begin(), secondRow.end());
                joinResult->writeRow<int>(joinRow);
                firstRow = firstCursor.getNext();
                while (!firstRow.empty() && firstRow[firstJoinColumn] == firstValue[firstJoinColumn])
                {
                    joinRow = firstRow;
                    joinRow.insert(joinRow.end(), secondValue.begin(), secondValue.end());
                    joinResult->writeRow<int>(joinRow);
                    firstRow = firstCursor.getNext();
                }
                secondRow = secondCursor.getNext();

                if (!secondRow.empty() && secondRow[secondJoinColumn] == secondValue[secondJoinColumn]){
                    firstCursor = firstCursor2;
                    firstRow = firstRow2;
                }
                else{
                    firstCursor2 = firstCursor;
                    firstRow2 = firstRow;
                }
            }
        }
        break;
    }
    case LESS_THAN:
    case LEQ:
    {
        while (!secondRow.empty())
        {
            firstCursor = firstTableCopy->getCursor();
            firstRow = firstCursor.getNext();
            while (!firstRow.empty() && evaluateBinOp(firstRow[firstJoinColumn], secondRow[secondJoinColumn], parsedQuery.joinBinaryOperator))
            {
                joinRow = firstRow;
                joinRow.insert(joinRow.end(), secondRow.begin(), secondRow.end());
                joinResult->writeRow<int>(joinRow);
                firstRow = firstCursor.getNext();
            }
            secondRow = secondCursor.getNext();
        }
        break;
    }

    case GREATER_THAN:
    case GEQ:
    {
        while (!firstRow.empty())
        {
            secondCursor = secondTableCopy->getCursor();
            secondRow = secondCursor.getNext();
            while (!secondRow.empty() && evaluateBinOp(firstRow[firstJoinColumn], secondRow[secondJoinColumn], parsedQuery.joinBinaryOperator))
            {
                joinRow = firstRow;
                joinRow.insert(joinRow.end(), secondRow.begin(), secondRow.end());
                joinResult->writeRow<int>(joinRow);
                secondRow = secondCursor.getNext();
            }
            firstRow = firstCursor.getNext();
        }
        break;
    }

    case NOT_EQUAL:
        break;
    }
    if (!joinResult->blockify())
    {
        cout << "ERROR: Failed to blockify join result" << endl;
        joinResult->unload();
        delete joinResult;
        return;
    }
    tableCatalogue.insertTable(joinResult);
    tableCatalogue.deleteTable(firstTableCopy->tableName);
    tableCatalogue.deleteTable(secondTableCopy->tableName);
    cout << "JOIN EXECUTED SUCCESSFULLY" << endl;
    return;
}