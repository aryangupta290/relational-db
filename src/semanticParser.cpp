#include "global.h"

bool semanticParse() {
    logger.log("semanticParse");
    switch (parsedQuery.queryType) {
        case CLEAR:
            return semanticParseCLEAR();
        case CROSS:
            return semanticParseCROSS();
        case DISTINCT:
            return semanticParseDISTINCT();
        case EXPORT:
            return semanticParseEXPORT();
        case INDEX:
            return semanticParseINDEX();
        case JOIN:
            return semanticParseJOIN();
        case LOAD_MATRIX:
            return semanticParseMATRIXLOAD();
        case PRINT_MATRIX:
            return semanticParseMATRIXPRINT();
        case TRANSPOSE_MATRIX:
            return semanticParseMATRIXTRANSPOSE();
        case RENAME_MATRIX:
            return semanticParseMATRIXRENAME();
        case EXPORT_MATRIX:
            return semanticParseMATRIXEXPORT();
        case CHECKSYMMETRY:
            return semanticParseCHECKSYMMETRY();
        case LIST:
            return semanticParseLIST();
        case LOAD:
            return semanticParseLOAD();
        case COMPUTE:
            return semanticParseCOMPUTE();
        case PRINT:
            return semanticParsePRINT();
        case PROJECTION:
            return semanticParsePROJECTION();
        case RENAME:
            return semanticParseRENAME();
        case SELECTION:
            return semanticParseSELECTION();
        case SORT:
            return semanticParseSORT();
        case SOURCE:
            return semanticParseSOURCE();
        default:
            cout << "SEMANTIC ERROR" << endl;
    }

    return false;
}