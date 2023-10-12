#ifndef __PAGE_H
#define __PAGE_H
#include "logger.h"
/**
 * @brief The Page object is the main memory representation of a physical page
 * (equivalent to a block). The page class and the page.h header file are at the
 * bottom of the dependency tree when compiling files.
 *<p>
 * Do NOT modify the Page class. If you find that modifications
 * are necessary, you may do so by posting the change you want to make on Moodle
 * or Teams with justification and gaining approval from the TAs.
 *</p>
 */

class Page {
   public:
    int columnCount;
    int rowCount;
    string pageName = "";
    vector<vector<int>> rows;
    vector<int> getRow(int rowIndex);
    Page();
    virtual ~Page(){};
    void writePage();
};

class TablePage : public Page {
   private:
    string tableName;
    int pageIndex;

   public:
    TablePage();
    TablePage(string tableName, int pageIndex);
    TablePage(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount);
};

class MatrixPage : public Page {
   private:
    string matrixName;
    int rowId;
    int colId;

   public:
    MatrixPage();
    MatrixPage(string matrixName, int rowId, int colId);
    MatrixPage(string matrixName, int rowId, int colId, vector<vector<int>>& rows);
};
typedef variant<Page, TablePage, MatrixPage> Pages;
#endif