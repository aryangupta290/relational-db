#include "global.h"
/**
 * @brief File contains method to process SORT commands.
 *
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 *
 * sorting_order = ASC | DESC
 */
bool syntacticParseSORT() {
    logger.log("syntacticParseSORT");
    if (tokenizedQuery[2] != "BY") {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = SORT;
    parsedQuery.sortRelationName = tokenizedQuery[1];
    bool ok = false;
    int i = 3;
    parsedQuery.sortColumnName.clear();
    parsedQuery.sortingStrategy.clear();
    for (i = 3; i < tokenizedQuery.size(); i++) {
        if (tokenizedQuery[i] == "IN") {
            ok = true;
            break;
        }
        parsedQuery.sortColumnName.push_back(tokenizedQuery[i]);
    }
    if (!parsedQuery.sortColumnName.size()) {
        cout << "SYNTAX ERROR: COLUMN NAMES TO SORT NOT PROVIDED\n";
        return false;
    }
    if (ok == false) {
        cout << "SYNTAX ERROR: NO 'IN' FOUND\n";
        return false;
    }
    for (i++; i < tokenizedQuery.size(); i++) {
        if (tokenizedQuery[i] == "ASC" || tokenizedQuery[i] == "DESC") {
            parsedQuery.sortingStrategy.push_back(tokenizedQuery[i]);
        } else {
            cout << "SYNTAX ERROR: ASC OR DSC NOT FOUND\n";
            return false;
        }
    }
    if (parsedQuery.sortingStrategy.size() != parsedQuery.sortColumnName.size()) {
        cout << "SYNTAX ERROR: SORTING STRATEGY SIZE UNEQUAL TO COLUMN NAMES\n";
        return false;
    }
    return true;
}

bool semanticParseSORT() {
    logger.log("semanticParseSORT");

    if (!tableCatalogue.isTable(parsedQuery.sortRelationName)) {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    for (string& columnName : parsedQuery.sortColumnName) {
        if (!tableCatalogue.isColumnFromTable(columnName, parsedQuery.sortRelationName)) {
            cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
            return false;
        }
    }
    return true;
}

void executeSORT(bool isExport = true) {
    logger.log("executeSORT");
    vector<int> colIndex;

    Table* table = tableCatalogue.getTable(parsedQuery.sortRelationName);
    for (auto& colName : parsedQuery.sortColumnName) {
        colIndex.push_back(table->getColumnIndex(colName));
    }
    const int maxRowsInBuffer = table->maxRowsPerBlock * BUFFER_SIZE;
    const int chunkSize = ((int)table->rowCount + maxRowsInBuffer - 1) / maxRowsInBuffer;
    Cursor cursor = table->getCursor();
    auto comp = [&](const vector<int>& a, const vector<int>& b) {
        for (int i = 0; i < colIndex.size(); i++) {
            int id = colIndex[i];
            if (a[id] == b[id]) continue;
            if (a[id] < b[id]) {
                if (parsedQuery.sortingStrategy[i] == "ASC")
                    return true;
                else
                    return false;
            } else {
                if (parsedQuery.sortingStrategy[i] == "ASC")
                    return false;
                else
                    return true;
            }
        }
        return false;
    };
    vector<Table*> sortedChunks;
    auto sortInternalBuckets = [&]() {
        for (int i = 0; i * maxRowsInBuffer < table->rowCount; i++) {
            sortedChunks.push_back(new Table(string(table->tableName + "_sort_" + to_string(i)), table->columns));
            tableCatalogue.insertTable(sortedChunks.back());
            vector<vector<int>> rows;
            for (int j = 0; j < min((int)table->rowCount - i * maxRowsInBuffer, maxRowsInBuffer); j++) {
                vector<int> r = cursor.getNext();
                rows.push_back(r);
            }
            sort(rows.begin(), rows.end(), comp);
            int ii = 0;
            while (ii < rows.size()) {
                int j = ii;
                vector<vector<int>> cur;
                for (; j < rows.size() && j < ii + table->maxRowsPerBlock; j++) {
                    cur.push_back(rows[j]);
                }
                sortedChunks.back()->rowsPerBlockCount.push_back(cur.size());
                BufferManager::writePageForTable(sortedChunks.back()->tableName, sortedChunks.back()->blockCount, cur, (int)cur.size());
                sortedChunks.back()->blockCount++;
                ii = j;
            }
        }
    };
    sortInternalBuckets();
    int freeChunks = BUFFER_SIZE - 1;
    int passes = (int)ceil(log(chunkSize) / log(freeChunks));
    for (int pass = 0; pass < passes; pass++) {
        // we have sortedChunks.size() tables which we need to merge
        vector<Table*> passChunks;
        for (int i = 0; i * freeChunks < (int)sortedChunks.size(); i++) {
            passChunks.push_back(new Table(string(table->tableName + "_pass_" + to_string(pass) + "_" + to_string(i)), table->columns));
            tableCatalogue.insertTable(passChunks.back());
            vector<vector<int>> rows, heap;
            vector<Cursor> cursors;
            for (int j = 0; j < min(freeChunks, (int)sortedChunks.size() - i * freeChunks); j++) {
                cursors.push_back(sortedChunks[i * freeChunks + j]->getCursor());
                heap.push_back(cursors.back().getNext());
            }
            while ((int)heap.size()) {
                int id = min_element(heap.begin(), heap.end(), comp) - heap.begin();
                rows.push_back(heap[id]);
                heap[id] = cursors[id].getNext();
                if (!heap[id].size()) {
                    heap.erase(heap.begin() + id);
                    cursors.erase(cursors.begin() + id);
                }
                if (rows.size() == table->maxRowsPerBlock) {
                    passChunks.back()->rowsPerBlockCount.push_back(rows.size());
                    BufferManager::writePageForTable(passChunks.back()->tableName, passChunks.back()->blockCount, rows, (int)rows.size());
                    passChunks.back()->blockCount++;
                    rows.clear();
                }
            }
            if ((int)rows.size()) {
                passChunks.back()->rowsPerBlockCount.push_back(rows.size());
                BufferManager::writePageForTable(passChunks.back()->tableName, passChunks.back()->blockCount, rows, (int)rows.size());
                passChunks.back()->blockCount++;
            }
        }
        for (int i = 0; i < sortedChunks.size(); i++) {
            tableCatalogue.deleteTable(sortedChunks[i]->tableName);
        }
        sortedChunks = passChunks;
    }
    Cursor f = sortedChunks[0]->getCursor();
    vector<int> row = f.getNext();
    vector<vector<int>> rows;
    int page_id = 0;
    while ((int)row.size()) {
        rows.push_back(row);
        if (rows.size() == table->maxRowsPerBlock) {
            BufferManager::writePageForTable(table->tableName, page_id, rows, rows.size());
            rows.clear();
            page_id++;
        }
        row = f.getNext();
    }
    if (rows.size()) {
        BufferManager::writePageForTable(table->tableName, page_id, rows, rows.size());
    }
    tableCatalogue.deleteTable(sortedChunks[0]->tableName);
    if (isExport)
        table->makePermanent();
    return;
}