#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
Page::Page() {
    this->pageName = "";
    this->rowCount = 0;
    this->columnCount = 0;
}

/**
 * @brief Construct a new Page:: Page object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName
 * @param pageIndex
 */
TablePage::TablePage() {}
TablePage::TablePage(string tableName, int pageIndex) {
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->tableName + "_Page" + to_string(pageIndex);
    Table table = *tableCatalogue.getTable(tableName);
    this->columnCount = table.columnCount;
    uint maxRowCount = table.maxRowsPerBlock;
    vector<int> row(columnCount, 0);
    this->rows.assign(maxRowCount, row);

    ifstream fin(pageName, ios::in);
    this->rowCount = table.rowsPerBlockCount[pageIndex];
    int number;
    for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++) {
        for (int columnCounter = 0; columnCounter < columnCount; columnCounter++) {
            fin >> number;
            this->rows[rowCounter][columnCounter] = number;
        }
    }
    fin.close();
}

/**
 * @brief Get row from page indexed by rowIndex
 *
 * @param rowIndex
 * @return vector<int>
 */
vector<int> Page::getRow(int rowIndex) {
    logger.log("Page::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->rows[rowIndex];
}

TablePage::TablePage(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount) {
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/" + this->tableName + "_Page" + to_string(pageIndex);
}

MatrixPage::MatrixPage(string matrixName, int rowId, int colId) {
    logger.log("Page::Page");
    this->matrixName = matrixName;
    this->rowId = rowId;
    this->colId = colId;
    this->rowCount = MATRIX_PAGE_DIMENSION;
    this->columnCount = MATRIX_PAGE_DIMENSION;
    this->pageName = "../data/temp/" + this->matrixName + "_Page" + to_string(this->rowId) + to_string(this->colId);
    this->rows = vector<vector<int>>(MATRIX_PAGE_DIMENSION, vector<int>(MATRIX_PAGE_DIMENSION, -1));
    ifstream readPageFile(this->pageName, ios ::in);
    if (!readPageFile.is_open()) {
        cerr << "Failed to open file: " << this->pageName << endl;
    } else {
        for (int i = 0; i < MATRIX_PAGE_DIMENSION; i++) {
            for (int j = 0; j < MATRIX_PAGE_DIMENSION; j++) {
                int temp;
                if (!(readPageFile >> temp)) {
                    cerr << "Error reading data from file." << endl;
                    // Handle the error, e.g., return or throw an exception
                }
                this->rows[i][j] = temp;  // -1's are also filled
            }
        }
        readPageFile.close();  // Close the file when done
    }
}

MatrixPage::MatrixPage(string matrixName, int rowId, int colId, vector<vector<int>>& rows) {
    logger.log("Page::Page");
    this->matrixName = matrixName;
    this->rowId = rowId;
    this->rowCount = MATRIX_PAGE_DIMENSION;
    this->columnCount = MATRIX_PAGE_DIMENSION;
    this->colId = colId;
    this->pageName = "../data/temp/" + this->matrixName + "_Page" + to_string(this->rowId) + to_string(this->colId);
    this->rows = rows;
}
/**
 * @brief writes current page contents to file.
 *
 */
void Page::writePage() {
    logger.log("Page::writePage");
    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++) {
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++) {
            if (columnCounter != 0)
                fout << " ";
            fout << this->rows[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
}
