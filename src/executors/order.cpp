#include "global.h"

/**
 * @brief File contains method to process ORDER commands.
 *
 * syntax:
 * <new_table> <- ORDER BY <attribute> ASC|DESC ON <table_name>
 */
bool syntacticParseORDER()
{
    logger.log("syntacticParseOrder");
    // cout << tokenizedQuery.size() <<" " << tokenizedQuery[3] << " " << tokenizedQuery[6] << " " << tokenizedQuery[5] << endl;
    if (tokenizedQuery.size() != 8 || tokenizedQuery[3] != "BY" || tokenizedQuery[6] != "ON" || (tokenizedQuery[5] != "ASC" && tokenizedQuery[5] != "DESC"))
    {
        cout << "SYNTAX ERROR oop" << endl;
        return false;
    }
    parsedQuery.queryType = ORDER;
    parsedQuery.orderRelationName = tokenizedQuery[7];
    parsedQuery.orderColumnName = tokenizedQuery[4];
    parsedQuery.orderStrategy = tokenizedQuery[6];
    parsedQuery.orderResultRelationName = tokenizedQuery[0];
    return true;
}

bool semanticParseORDER()
{
    logger.log("semanticParseORDER");
    if (tableCatalogue.isTable(parsedQuery.orderResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }
    if (!tableCatalogue.isTable(parsedQuery.orderRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    if (!tableCatalogue.isColumnFromTable(parsedQuery.orderColumnName, parsedQuery.orderRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }

    return true;
}

void executeORDER()
{
    logger.log("executeORDER");

    Table table = *tableCatalogue.getTable(parsedQuery.orderRelationName);
    Table *resultantTable = new Table(parsedQuery.orderResultRelationName, table.columns);
    Cursor cursor = table.getCursor();
    vector<int> row = cursor.getNext();
    while (!row.empty())
    {
        resultantTable->writeRow<int>(row);
        row = cursor.getNext();
    }
    if (resultantTable->blockify())
    {
        tableCatalogue.insertTable(resultantTable);
        parsedQuery.sortRelationName = parsedQuery.orderResultRelationName;
        parsedQuery.sortColumnName.clear();
        parsedQuery.sortingStrategy.clear();
        parsedQuery.sortColumnName.push_back(parsedQuery.orderColumnName);
        parsedQuery.sortingStrategy.push_back(parsedQuery.orderStrategy);
        executeSORT(false);
        cout << "ORDER EXECUTED SUCCESSFULLY\n";
    }
    else
    {
        cout << "Empty Table" << endl;
        resultantTable->unload();
        delete resultantTable;
    }
    return;
}