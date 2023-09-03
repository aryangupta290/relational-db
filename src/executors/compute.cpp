#include "global.h"
/**
 * @brief
 * SYNTAX RENAME Matix
 */
bool syntacticParseCOMPUTE() {
    logger.log("syntacticParseCOMPUTE");
    if (tokenizedQuery.size() != 2) {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = COMPUTE;
    parsedQuery.computeFileName = tokenizedQuery[1];
    return true;
}

bool semanticParseCOMPUTE() {
    logger.log("semanticParseCOMPUTE");
    if (!matrixCatalogue.isMatrix(parsedQuery.computeFileName)) {
        cout << "SEMANTIC ERROR: Matrix does't exist" << endl;
        return false;
    }
    if (matrixCatalogue.isMatrix(parsedQuery.computeFileName + "_RESULT")) {
        cout << "SEMANTIC ERROR:Resultant Matrix already exists" << endl;
        return false;
    }
    return true;
}

void executeCompute() {
    logger.log("executeMatrixCompute");

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.computeFileName);
    matrix->compute();
}