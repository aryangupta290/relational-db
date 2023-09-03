#include "global.h"
/**
 * @brief
 * SYNTAX RENAME Matix
 */
bool syntacticParseMATRIXRENAME() {
    logger.log("syntacticParseMATRIXRENAME");
    if (tokenizedQuery.size() != 4) {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = RENAME_MATRIX;
    parsedQuery.renameMatrixFromFileName = tokenizedQuery[2];
    parsedQuery.renameMatrixToFileName = tokenizedQuery[3];
    return true;
}

bool semanticParseMATRIXRENAME() {
    logger.log("semanticParseMATRIXRENAME");
    if (!matrixCatalogue.isMatrix(parsedQuery.renameMatrixFromFileName)) {
        cout << "SEMANTIC ERROR: Matrix does't exist" << endl;
        return false;
    }
    if (matrixCatalogue.isMatrix(parsedQuery.renameMatrixToFileName)) {
        cout << "SEMANTIC ERROR: Matrix already exist" << endl;
        return false;
    }
    return true;
}

void executeMatrixRename() {
    logger.log("executeMatrixRENAME");

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.renameMatrixFromFileName);
    matrix->rename(parsedQuery.renameMatrixToFileName);
}