#include "global.h"
/**
 * @brief
 * SYNTAX RENAME Matix
 */
bool syntacticParseCHECKSYMMETRY() {
    logger.log("syntacticParseCHECKSYMMETRY");
    if (tokenizedQuery.size() != 2) {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = CHECKSYMMETRY;
    parsedQuery.checkSymmetryMatrixFileName = tokenizedQuery[1];
    return true;
}

bool semanticParseCHECKSYMMETRY() {
    logger.log("semanticParseCHECKSYMMETRY");
    if (!matrixCatalogue.isMatrix(parsedQuery.checkSymmetryMatrixFileName)) {
        cout << "SEMANTIC ERROR: Matrix does't exist" << endl;
        return false;
    }
    return true;
}

void executeCheckSymmetry() {
    logger.log("executeMatrixCheckSymmetry");

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.checkSymmetryMatrixFileName);
    matrix->checksymmetry();
}