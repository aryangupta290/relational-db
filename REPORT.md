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
1. Efficient Data Organization: The importance of good data organization strategy and page design. This was specifically useful for the optimisation of transpose-related tasks, so we divided **NxN** matrix into smaller matrices, as mentioned earlier.
We also made sure that the memory was being used efficiently under the given constraints and use cases.
2. Understanding the existing codebase: Understanding how exactly the table is being implemented was really important so as to think of how we can do similar operations for matrices and error handling.
3. Importance of OOPS: Since the codebase was very modular and used OOPS principles, we got familiar with the codebase very quickly and could work on different parts of the code together without much issue. We tried to implement these principles wherever we could in our code.
4. Team Collaboration: Since the tasks were very closely linked to each other, proper team collaboration was the key to completing this phase on time. Through proper discussion and brainstorming, we understood the initial code, came up with the optimal strategy to store the matrix, figured out what all changes had to be made, and came up with implementation logic and details for all tasks. This significantly reduced any errors, that would have been made if these tasks were done individually.
5. Code reviews and testing: Reviewing the codes of other team members and testing their parts made sure that none of the edge cases were missed.

## CONTRIBUTIONS:
We all collaborated and came up with the design and implementation logic for all the parts together. Any issues with the logic were resolved collaboratively through discussion among teammates.
Changes in buffermanager, page file, and blockify function were also made together since these were the backbone for this phase.
For the coding part, this was the division of work:
ARYAN - load and transpose
PRATHAM - print, rename and checksymmetry
ANMOL - export and compute
(implementation details for these were discussed among teammates, so as to reduce errors and find the best approach).
