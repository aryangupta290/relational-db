#include "global.h"
/**
 * @brief
 * SYNTAX EXPORT Matix
 */
bool syntacticParseMATRIXEXPORT() {
    logger.log("syntacticParseMATRIXEXPORT");
    if (tokenizedQuery.size() != 3) {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = EXPORT_MATRIX;
    parsedQuery.exportMatrixFileName = tokenizedQuery[2];
    return true;
}

bool semanticParseMATRIXEXPORT() {
    logger.log("semanticParseMATRIXEXPORT");
    if (!matrixCatalogue.isMatrix(parsedQuery.exportMatrixFileName)) {
        cout << "SEMANTIC ERROR: Matrix does't exist" << endl;
        return false;
    }
    return true;
}

void executeMatrixExport() {
    logger.log("executeMatrixEXPORT");

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.exportMatrixFileName);
    matrix->export_mat();
}