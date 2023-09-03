#include "global.h"
/**
 * @brief
 * SYNTAX: TRANSPOSE Matix
 */
bool syntacticParseMATRIXTRANSPOSE() {
    logger.log("syntacticParseMATRIXTRANSPOSE");
    if (tokenizedQuery.size() != 3) {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = TRANSPOSE_MATRIX;
    parsedQuery.transposeMatrixFileName = tokenizedQuery[2];
    return true;
}

bool semanticParseMATRIXTRANSPOSE() {
    logger.log("semanticParseMATRIXTRANSPOSE");
    if (!matrixCatalogue.isMatrix(parsedQuery.transposeMatrixFileName)) {
        cout << "SEMANTIC ERROR: Matrix does't exist" << endl;
        return false;
    }
    return true;
}

void executeMatrixTranspose() {
    logger.log("executeMatrixTRANSPOSE");

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.transposeMatrixFileName);
    matrix->transpose();
}