#include "global.h"
/**
 * @brief
 * SYNTAX: LOAD Matix
 */
bool syntacticParseMATRIXPRINT() {
    logger.log("syntacticParseMATRIXPrint");
    if (tokenizedQuery.size() != 3) {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = PRINT_MATRIX;
    parsedQuery.printMatrixFileName = tokenizedQuery[2];
    return true;
}

bool semanticParseMATRIXPRINT() {
    logger.log("semanticParseMATRIXPRINT");
    if (!matrixCatalogue.isMatrix(parsedQuery.printMatrixFileName)) {
        cout << "SEMANTIC ERROR: Matrix does't exist" << endl;
        return false;
    }
    return true;
}

void executeMatrixPrint() {
    logger.log("executeMatrixPrint");

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.printMatrixFileName);
    matrix->print();
}