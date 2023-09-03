#include "global.h"
/**
 * @brief
 * SYNTAX: LOAD Matix
 */
bool syntacticParseMATRIXLOAD() {
    logger.log("syntacticParseMATRIXLoad");
    if (tokenizedQuery.size() != 3) {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = LOAD_MATRIX;
    parsedQuery.loadMatrixFileName = tokenizedQuery[2];
    return true;
}

bool semanticParseMATRIXLOAD() {
    logger.log("semanticParseMATRIXLOAD");
    if (matrixCatalogue.isMatrix(parsedQuery.loadMatrixFileName)) {
        cout << "SEMANTIC ERROR: Matrix already exists" << endl;
        return false;
    }
    if (!isFileExists(parsedQuery.loadMatrixFileName)) {
        cout << "SEMANTIC ERROR: Matrix file doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeMatrixLoad() {
    logger.log("executeMatrixLoad");

    Matrix *matrix = new Matrix(parsedQuery.loadMatrixFileName);
    if (matrix->load()) {
        matrixCatalogue.insertMatrix(matrix);
        cout << "Loaded Table. Dimenstion: " << matrix->dimension << endl;
    }
}