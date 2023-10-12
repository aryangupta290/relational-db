#include "matrixCatalogue.h"
#include "tableCatalogue.h"

using namespace std;

enum QueryType {
    CLEAR,
    CROSS,
    DISTINCT,
    EXPORT,
    INDEX,
    JOIN,
    LIST,
    LOAD,
    LOAD_MATRIX,
    PRINT_MATRIX,
    TRANSPOSE_MATRIX,
    RENAME_MATRIX,
    CHECKSYMMETRY,
    EXPORT_MATRIX,
    COMPUTE,
    PRINT,
    PROJECTION,
    RENAME,
    SELECTION,
    SORT,
    SOURCE,
    UNDETERMINED
};

enum BinaryOperator {
    LESS_THAN,
    GREATER_THAN,
    LEQ,
    GEQ,
    EQUAL,
    NOT_EQUAL,
    NO_BINOP_CLAUSE
};

enum SortingStrategy {
    ASC,
    DESC,
    NO_SORT_CLAUSE
};

enum SelectType {
    COLUMN,
    INT_LITERAL,
    NO_SELECT_CLAUSE
};

class ParsedQuery {
   public:
    QueryType queryType = UNDETERMINED;

    string clearRelationName = "";

    string crossResultRelationName = "";
    string crossFirstRelationName = "";
    string crossSecondRelationName = "";

    string distinctResultRelationName = "";
    string distinctRelationName = "";

    string exportRelationName = "";

    IndexingStrategy indexingStrategy = NOTHING;
    string indexColumnName = "";
    string indexRelationName = "";

    BinaryOperator joinBinaryOperator = NO_BINOP_CLAUSE;
    string joinResultRelationName = "";
    string joinFirstRelationName = "";
    string joinSecondRelationName = "";
    string joinFirstColumnName = "";
    string joinSecondColumnName = "";

    string loadRelationName = "";

    string printRelationName = "";

    string projectionResultRelationName = "";
    vector<string> projectionColumnList;
    string projectionRelationName = "";

    string renameFromColumnName = "";
    string renameToColumnName = "";
    string renameRelationName = "";

    SelectType selectType = NO_SELECT_CLAUSE;
    BinaryOperator selectionBinaryOperator = NO_BINOP_CLAUSE;
    string selectionResultRelationName = "";
    string selectionRelationName = "";
    string selectionFirstColumnName = "";
    string selectionSecondColumnName = "";
    int selectionIntLiteral = 0;

    vector<string> sortingStrategy;
    vector<string> sortColumnName;
    string sortRelationName = "";

    string loadMatrixFileName = "";
    string printMatrixFileName = "";
    string transposeMatrixFileName = "";
    string renameMatrixFromFileName = "";
    string renameMatrixToFileName = "";
    string checkSymmetryMatrixFileName = "";
    string computeFileName = "";
    string exportMatrixFileName = "";

    string sourceFileName = "";

    ParsedQuery();
    void clear();
};

bool syntacticParse();
bool syntacticParseCLEAR();
bool syntacticParseCROSS();
bool syntacticParseDISTINCT();
bool syntacticParseEXPORT();
bool syntacticParseINDEX();
bool syntacticParseJOIN();
bool syntacticParseLIST();
bool syntacticParseLOAD();
bool syntacticParsePRINT();
bool syntacticParsePROJECTION();
bool syntacticParseRENAME();
bool syntacticParseSELECTION();
bool syntacticParseSORT();
bool syntacticParseSOURCE();
bool syntacticParseMATRIXLOAD();
bool syntacticParseMATRIXPRINT();
bool syntacticParseMATRIXTRANSPOSE();
bool syntacticParseMATRIXRENAME();
bool syntacticParseMATRIXEXPORT();
bool syntacticParseCHECKSYMMETRY();
bool syntacticParseCOMPUTE();

bool isFileExists(string tableName);
bool isQueryFile(string fileName);
