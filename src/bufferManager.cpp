#include "global.h"

BufferManager::BufferManager() {
    logger.log("BufferManager::BufferManager");
}

/**
 * @brief Function called to read a page from the buffer manager. If the page is
 * not present in the pool, the page is read and then inserted into the pool.
 *
 * @param tableName
 * @param pageIndex
 * @return Page
 */
TablePage BufferManager::getTablePage(string tableName, int pageIndex) {
    logger.log("BufferManager::getPage");
    string pageName = "../data/temp/" + tableName + "_Page" + to_string(pageIndex);
    if (this->inPool(pageName)) {
        return get<TablePage>(this->getFromPool(pageName));
    } else
        return this->insertTableIntoPool(tableName, pageIndex);
}

/**
 * @brief Function called to read a page from the buffer manager. If the page is
 * not present in the pool, the page is read and then inserted into the pool.
 *
 * @param matrixName
 * @param rowId
 * @param colId
 * @return Page
 */
MatrixPage BufferManager::getMatrixPage(string matrixName, int rowId, int colId) {
    logger.log("BufferManager::getPage");
    string pageName = "../data/temp/" + matrixName + "_Page" + to_string(rowId) + to_string(colId);
    if (this->inPool(pageName)) {
        return get<MatrixPage>(this->getFromPool(pageName));
    } else {
        return this->insertMatrixIntoPool(matrixName, rowId, colId);
    }
}

/**
 * @brief Checks to see if a page exists in the pool
 *
 * @param pageName
 * @return true
 * @return false
 */
bool BufferManager::inPool(string pageName) {
    logger.log("BufferManager::inPool");
    for (auto page : this->pages) {
        if (pageName == getPageName(page))
            return true;
    }
    return false;
}

/**
 * @brief If the page is present in the pool, then this function returns the
 * page. Note that this function will fail if the page is not present in the
 * pool.
 *
 * @param pageName
 * @return Page
 */
Pages BufferManager::getFromPool(string pageName) {
    logger.log("BufferManager::getFromPool");
    for (auto page : this->pages)
        if (pageName == getPageName(page))
            return page;
}

/**
 * @brief Inserts page indicated by tableName and pageIndex into pool. If the
 * pool is full, the pool ejects the oldest inserted page from the pool and adds
 * the current page at the end. It naturally follows a queue data structure.
 *
 * @param tableName
 * @param pageIndex
 * @return Page
 */
TablePage BufferManager::insertTableIntoPool(string tableName, int pageIndex) {
    logger.log("BufferManager::insertIntoPool");
    TablePage page(tableName, pageIndex);
    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(page);
    return page;
}

/**
 * @brief Inserts page indicated by matrixName, rowId and colId into pool. If the
 * pool is full, the pool ejects the oldest inserted page from the pool and adds
 * the current page at the end. It naturally follows a queue data structure.
 *
 * @param matrixName
 * @param rowId
 * @param colId
 * @return Page
 */
MatrixPage BufferManager::insertMatrixIntoPool(string matrixName, int rowId, int colId) {
    logger.log("BufferManager::insertIntoPool");
    MatrixPage page(matrixName, rowId, colId);
    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(page);
    return page;
}

/**
 * @brief The buffer manager is also responsible for writing pages. This is
 * called when new tables are created using assignment statements.
 *
 * @param tableName
 * @param pageIndex
 * @param rows
 * @param rowCount
 */
void BufferManager::writePageForTable(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount) {
    logger.log("BufferManager::writePage");
    TablePage page(tableName, pageIndex, rows, rowCount);
    page.writePage();
}

/**
 * @brief The buffer manager is also responsible for writing pages. This is
 * called when new matrices are created using assignment statements. If the
 * page already exists in buffer, it's rows is updated with the updated rows.
 * This is not required for table since there is no update operation in Tables.
 *
 * @param matrixName
 * @param rowId
 * @param colId
 * @param rows
 */
void BufferManager::writePageForMatrix(string matrixName, int rowId, int colId, vector<vector<int>>& rows) {
    logger.log("BufferManager::writePage");
    MatrixPage page(matrixName, rowId, colId, rows);
    for (auto& pg : this->pages)
        if (page.pageName == getPageName(pg)) {
            get<MatrixPage>(pg).rows = rows;
        }
    page.writePage();
}

/**
 * @brief Deletes file names fileName
 *
 * @param fileName
 */
void BufferManager::deleteFile(string fileName) {
    if (remove(fileName.c_str()))
        logger.log("BufferManager::deleteFile: Err");
    else
        logger.log("BufferManager::deleteFile: Success");
}

/**
 * @brief Overloaded function that calls deleteFile(fileName) by constructing
 * the fileName from the tableName and pageIndex.
 *
 * @param tableName
 * @param pageIndex
 */
void BufferManager::deletePageForTable(string tableName, int pageIndex) {
    logger.log("BufferManager::deleteFile");
    string fileName = "../data/temp/" + tableName + "_Page" + to_string(pageIndex);
    this->deleteFile(fileName);
}

/**
 * @brief Overloaded function that calls deleteFile(fileName) by constructing
 * the fileName from the tableName and pageIndex.
 *
 * @param matrixName
 * @param rowId
 * @param colId
 */
void BufferManager::deletePageForMatrix(string matrixName, int rowId, int colId) {
    logger.log("BufferManager::deleteFile");
    string fileName = "../data/temp/" + matrixName + "_Page" + to_string(rowId) + to_string(colId);
    this->deleteFile(fileName);
}

string getPageName(const Pages& page) {
    if (holds_alternative<Page>(page)) {
        const Page& pageVariant = std::get<Page>(page);
        return pageVariant.pageName;
    } else if (std::holds_alternative<TablePage>(page)) {
        const TablePage& tablePageVariant = std::get<TablePage>(page);
        return tablePageVariant.pageName;
    } else if (std::holds_alternative<MatrixPage>(page)) {
        const MatrixPage& matrixPageVariant = std::get<MatrixPage>(page);
        return matrixPageVariant.pageName;
    }
}