#include "semanticParser.h"

void executeCommand();

void executeCLEAR();
void executeCROSS();
void executeDISTINCT();
void executeEXPORT();
void executeINDEX();
void executeJOIN();
void executeLIST();
void executeLOAD();
void executePRINT();
void executePROJECTION();
void executeMatrixLoad();
void executeMatrixPrint();
void executeMatrixTranspose();
void executeMatrixRename();
void executeMatrixExport();
void executeCheckSymmetry();
void executeCompute();
void executeRENAME();
void executeSELECTION();
void executeSORT();
void executeSOURCE();

bool evaluateBinOp(int value1, int value2, BinaryOperator binaryOperator);
void printRowCount(int rowCount);