#include <global.h>
// For rename, remove it from catalogue and then add it again
void MatrixCatalogue::insertMatrix(Matrix* matrix) {
    logger.log("MatrixCatalogue::~insertMatrix");
    this->matrices[matrix->matrixName] = matrix;
}
void MatrixCatalogue::eraseMatrix(string matrixName) {
    logger.log("MatrixCatalogue::~eraseMatrix");
    this->matrices.erase(matrixName);
}

Matrix* MatrixCatalogue::getMatrix(string matrixName) {
    logger.log("MatrixCatalogue::getMatrix");
    Matrix* matrix = this->matrices[matrixName];
    return matrix;
}

bool MatrixCatalogue::isMatrix(string matrixName) {
    logger.log("MatrixCatalogue::isMatrix");
    if (this->matrices.count(matrixName))
        return true;
    return false;
}

MatrixCatalogue::~MatrixCatalogue() {
    logger.log("MatrixCatalogue::~MatrixCatalogue");
    for (auto mat : this->matrices) {
        mat.second->unload();
        delete mat.second;
    }
}