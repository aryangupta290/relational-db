#include "global.h"

void executeCommand() {
    switch (parsedQuery.queryType) {
        case CLEAR:
            executeCLEAR();
            break;
        case CROSS:
            executeCROSS();
            break;
        case DISTINCT:
            executeDISTINCT();
            break;
        case EXPORT:
            executeEXPORT();
            break;
        case INDEX:
            executeINDEX();
            break;
        case JOIN:
            executeJOIN();
            break;
        case LIST:
            executeLIST();
            break;
        case LOAD:
            executeLOAD();
            break;
        case LOAD_MATRIX:
            executeMatrixLoad();
            break;
        case PRINT:
            executePRINT();
            break;
        case PRINT_MATRIX:
            executeMatrixPrint();
            break;
        case TRANSPOSE_MATRIX:
            executeMatrixTranspose();
            break;
        case RENAME_MATRIX:
            executeMatrixRename();
            break;
        case EXPORT_MATRIX:
            executeMatrixExport();
            break;
        case CHECKSYMMETRY:
            executeCheckSymmetry();
            break;
        case COMPUTE:
            executeCompute();
            break;
        case PROJECTION:
            executePROJECTION();
            break;
        case RENAME:
            executeRENAME();
            break;
        case SELECTION:
            executeSELECTION();
            break;
        case SORT:
            executeSORT(true);
            break;
        case SOURCE:
            executeSOURCE();
            break;
        case ORDER:
            executeORDER();
            break;
        case GROUP:
            executeGROUP();
            break;
        default:
            cout << "PARSING ERROR" << endl;
    }

    return;
}

void printRowCount(int rowCount) {
    cout << "\n\nRow Count: " << rowCount << endl;
    return;
}