#ifndef __MATRIX_H
#define __MATRIX_H

#include "bufferManager.h"

class Matrix {
   public:
    string sourceFileName = "";
    string hashedName = "";
    string matrixName = "";
    uint dimension;
    uint blockCountSqrt;
    uint blockCount;

    Matrix();
    Matrix(string matrixName);
    bool load();
    bool blockify();
    void print();
    void transpose();
    void rename(string newFileName);
    void checksymmetry();
    void compute();
    void export_mat();
    void unload();
};

#endif