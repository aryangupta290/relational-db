#PROJECT PHASE 1

## PAGE DESIGN

We split the **NxN** matrix into smaller matrics, each of size `(int)sqrt((BLOCK_SIZE * 1024) / sizeof(int))`, say M . The reason we do is it easily allows us to perform the tranpose operations. Therefore the matrix is stored as smaller matrices into blocks, each of size 16. We pad the extra space with 0.

# COMMAND: LOAD MATRIX

It first calls the constructor, which initiates the matrix class, and call blockify. It splits the matrix into smaller 16\*16 matrices to pages and writes them into the pages (identified using matrixName,rowId and colId).

Checks: We check that should not exist already in the database.

Number of blocks to write: (dimension/M) \* (dimension/M)
Number of blocks to read: 0

# COMMAND: PRINT MATRIX

We just go through the first 4 blocks, extract the matrix using the getMatrixPage function and store them into a vector.

Checks: We check that matrix should exist in the database.

Number of blocks to write: 0
Number of blocks to read: 4

# COMMAND: TRANSPOSE MATRIX

We go through upper triangular portion of the matrix, and then for each corresponding matrics, we first transpose them within themself and then swap them. This ensures that the matrix gets transposed.

Checks: We check that matrix should exist in the database.

Number of blocks to write: (dimension/M) \* (dimension/M)
Number of blocks to read: (dimension/M) \* (dimension/M)

# COMMAND: RENAME MATRIX

Trivial, we just update the matrixName.

Checks: We check that matrix should exist in the database and the new name should not.

Number of blocks to write: 0
Number of blocks to read: 0

# COMMAND: EXPORT MATRIX

We go through all the blocks, extract the matrix using the getMatrixPage function and store them into a vector.
In one go, we read M rows and write them together into the file.
This becomes possible because maximum matrix size is 1e4, so we can easily allocate around 1e4 \* 16 memory in the stack.

Checks: We check that matrix should exist in the database.

Number of blocks to write: 0
Number of blocks to read: (dimension/M) \* (dimension/M)

# COMMAND: CHECKSYMMETRY

Similar to what we did in tranpose, we find the transpose matrix for each block and just check if all entries are same. If yes, then matrix is symmetric, else we print false.

Checks: We check that matrix should exist in the database.

Number of blocks to write: 0
Number of blocks to read: (dimension/M) \* (dimension/M)

# COMMAND: COMPUTE

Again, similar to tranpose, we find the transpose for each block and get their difference. After that, similar to load function, we write them into the result file. 

Checks: We check that matrix should exist in the database and no matrix with the result name should exist.

Number of blocks to write: (dimension/M) \* (dimension/M)
Number of blocks to read: (dimension/M) \* (dimension/M)

## LEARNINGS:

## CONTRIBUTIONS:
