#include "executor.h"

extern float BLOCK_SIZE;
extern uint BLOCK_COUNT;
extern uint PRINT_COUNT;
extern uint BUFFER_SIZE;
extern uint MATRIX_PAGE_DIMENSION;
extern long long int num_blocks_read;
extern long long int num_blocks_write;
extern vector<string> tokenizedQuery;
extern ParsedQuery parsedQuery;
extern TableCatalogue tableCatalogue;
extern MatrixCatalogue matrixCatalogue;
extern BufferManager bufferManager;