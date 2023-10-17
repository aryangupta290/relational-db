# PROJECT PHASE 2

## COMMAND: SORT
We have implemented the standard external merge sort algorithm for sorting relations in a database, that are large datasets that don't fit in memory.

1. **Initializations**:
   - The given table and columns to be sorted are identified.
   - The maximum number of rows that can fit in the buffer is calculated.
   - `chunkSize` represents the total number of chunks (or partitions) the table will be split into.

2. **Comparison Function**:
   - The `comp` lambda function is a comparator that compares two rows based on the sorting columns and their respective sorting strategy (ascending or descending).

3. **Split & Sort Phase (sortInternalBuckets)**:
   - This phase divides the data into manageable chunks that fit in the buffer.
   - Each chunk is then sorted internally and stored as a separate "sorted chunk".
   - `sortedChunks` is a vector containing pointers to these sorted chunk tables.

4. **Merge Phase**:
   - The idea here is to merge multiple sorted chunks into larger sorted chunks until the entire relation is sorted.
   - A certain number of sorted chunks (determined by `freeChunks`) are merged together in each pass.
   - The merging utilizes a heap structure (the `heap` vector) to identify the smallest (or largest) element among the chunks to be merged.
   - The number of passes required is determined by the `passes` calculation.
   - After each pass, old sorted chunks are deleted and replaced by the merged larger chunks.

5. **Final Write Back**:
   - Once the entire relation is sorted, the sorted rows are written back to the main table.
   - If there are any old sorted chunks remaining, they are deleted.

6. **Post-processing**:
   - Buffer pages are cleared.
   - If `isExport` is true, the table's changes are made permanent and a success message is printed.

The code is optimized for disk-based operations. It reads and writes data in blocks/pages to minimize the I/O operations. The `BufferManager` is responsible for handling the reading and writing of pages.

## COMMAND: ORDER BY

 In essence, the implementation of the `ORDER BY` functionality is a wrapper around the external sorting function.

Here's a breakdown of the function:

1. **Initialization**:
   - Creates a new table (`resultantTable`) which will hold the ordered rows.
   - It initializes a cursor on the original table and iteratively fetches rows from it.
   - Each fetched row is written to the `resultantTable`, which essentially creates a copy of the original table.

2. **External Sorting**:
   - Calls the `executeSORT(false)` function, which will now sort the `resultantTable` based on the given column and strategy (ascending or descending).
   - The `false` argument indicates that the table should not be made permanent after sorting (i.e., it's kept as a temporary table).

3. **Post-processing**:
   - Once sorted, the function calls `makeTemp()` on the `resultantTable` rerwrites the csv in the temp folder for reference purposes, (this was non essential).

## COMMAND: GROUP BY - HAVING

The `executeGROUP()` function first sorts the table based on the group column, then partitions the rows into groups based on distinct values of the group column. Each group is then aggregated based on the specified function, and the results are filtered based on a condition applied to the aggregated value. The final result is stored in a new table. The function makes use of the `executeSORT()` function to sort the table, and the overall logic effectively implements the SQL "GROUP BY" operation with an optional "HAVING" clause.

1. **Initialization**:
    - Creates a new table (`sortedTable`) to hold the rows of the table in sorted order based on the group column.
    - Rows from the original table are copied to the `sortedTable` using a cursor.

2. **Sorting `sortedTable`**:
    - The table is blockified (divided into pages or blocks). 
    - It is then sorted based on the column specified for grouping by invoking `executeSORT(false)`.

3. **Partitioning into Groups**:
    - Rows in the `sortedTable` are partitioned into different groups based on the distinct values in the grouping column.
    - Each group is stored as a separate table with a name like `partition_<value>` in the `tableCatalogue`.

4. **Aggregating  and filtering the Groups**:
    - For each group, the specified aggregation function (SUM, MAX, MIN, COUNT, AVG) is applied to the column specified in the `parsedQuery.groupReturnColumnName`.
    - Results are stored in `retRow` which is then added to the `resultantTable`.
    - If the `HAVING` clausecondition is satisfied, the row is added to the `resultantTable`.
    - The `resultantTable` is blockified.

## COMMAND: JOIN

In essence, the `executeJOIN()` function Creates sorted copies of the two tables based on the join columns. Uses the sort-merge join algorithm to merge these tables based on the specified join condition and Stores the results in a new table.

1. **Initialization**:
    - We fetch the two tables to be joined and create copies of both tables.

2. **Sorting Table Copies**:
    - Both table copies are sorted based on the join columns.
    - Sorting is achieved by updating the `parsedQuery` properties and calling the `executeSORT(false)` function. This means the sorting logic is encapsulated in the `executeSORT` function.

3. **Joining Sorted Tables**:
    - A resultant table (`joinResult`) is created to hold the join results. Its header is a combination of the columns from both tables.
    - This uses cursors to traverse the sorted table copies.
    - Depending on the join condition, rows from both tables are compared:
      - **EQUAL condition**: Matches rows with equal join column values. 
        If multiple rows in the first table match with multiple rows in the second table, it produces a Cartesian product of these rows. Two cursors (`firstCursor` and `firstCursor2`) for the first table are used to achieve this.
      - **LESS_THAN or LEQ**: For every row in the second table, matches with every row in the first table satisfying the less than or less than equal condition.
      - **GREATER_THAN or GEQ**: For every row in the first table, matches with every row in the second table satisfying the greater than or greater than equal condition.
    - The matched rows are combined and added to the `joinResult` table.
    - Other join conditions like `NOT_EQUAL` could be added in the future as seen in the switch case.
    - The `joinResult` table is blockified.


>## LEARNINGS:
>After implementing the commands for sort, order, group, and join, here are the key learnings we've acquired:
>1. **Understanding of Database Operations**: We've delved deep into some fundamental database operations, understanding how they work at a low level, using cursors as data pointers for iterative data retrieval.
>2. **Memory Constraints & External Sorting**: For operations like sorting and joining, data might not always fit into memory. We've practically implemented external sorting using the sort-merge join algorithm, a technique to handle such situations.
>3. **Algorithmic Thinking**: Implementing these commands has honed our algorithmic thinking, especially when optimizing for constraints like memory usage and speed. Certain operations, like joins, can be computationally intensive. Understanding their complexity and potential optimizations is crucial for building efficient database systems.
>4. **Modular Coding**: The code is structured in functions (`executeSORT`, `executeJOIN`, etc.), promoting reusability and readability.
>5. **Intermediate Data Storage**: Creating temporary or intermediate tables (like sorted copies of original tables) is sometimes necessary to simplify and break down complex operations.
>6. **Database Design Patterns**: Concepts like table catalogues, which hold metadata about tables, showcase common patterns and best practices in database system design.
>7. **Debugging & Testing**: With such intricate operations, debugging and testing become essential skills. We've likely honed these skills by ensuring each function works as expected and handles edge cases.Hence we have code a basic testsuit generator.

> ## CONTRIBUTIONS
> 
> Completeing the proect required implementing the merge sort algorithm and algorithms for all the part 3 commands that utilized the sort function. Hence We all collaborated and came up with the design and implementation logic for all the parts together. Any issues with the logic were resolved collaboratively through discussion among teammates.
> For the coding part, this was the division of work:
>>ARYAN - External Sort
>
>>PRATHAM - Group, Join
>
>>ANMOL - Sort, Order, Testing
>
>(implementation details for these were discussed among teammates, so as to reduce errors and find the best approach).

# PROJECT PHASE 1

## PAGE DESIGN

We split the **NxN** matrix into smaller matrics, each of size `(int)sqrt((BLOCK_SIZE * 1024) / sizeof(int))`, say M . The reason we do is it easily allows us to perform the tranpose operations. Therefore the matrix is stored as smaller matrices into blocks, each of size 16. We pad the extra space with 0.

## COMMAND: LOAD MATRIX

It first calls the constructor, which initiates the matrix class, and call blockify. It splits the matrix into smaller 16\*16 matrices to pages and writes them into the pages (identified using matrixName,rowId and colId).

Checks: We check that should not exist already in the database.

Number of blocks to write: (dimension/M) \* (dimension/M)
Number of blocks to read: 0

## COMMAND: PRINT MATRIX

We just go through the first 4 blocks, extract the matrix using the getMatrixPage function and store them into a vector.

Checks: We check that matrix should exist in the database.

Number of blocks to write: 0
Number of blocks to read: 4

## COMMAND: TRANSPOSE MATRIX

We go through upper triangular portion of the matrix, and then for each corresponding matrics, we first transpose them within themself and then swap them. This ensures that the matrix gets transposed.

Checks: We check that matrix should exist in the database.

Number of blocks to write: (dimension/M) \* (dimension/M)
Number of blocks to read: (dimension/M) \* (dimension/M)

## COMMAND: RENAME MATRIX

Trivial, we just update the matrixName.

Checks: We check that matrix should exist in the database and the new name should not.

Number of blocks to write: 0
Number of blocks to read: 0

## COMMAND: EXPORT MATRIX

We go through all the blocks, extract the matrix using the getMatrixPage function and store them into a vector.
In one go, we read M rows and write them together into the file.
This becomes possible because maximum matrix size is 1e4, so we can easily allocate around 1e4 \* 16 memory in the stack.

Checks: We check that matrix should exist in the database.

Number of blocks to write: 0
Number of blocks to read: (dimension/M) \* (dimension/M)

## COMMAND: CHECKSYMMETRY

Similar to what we did in tranpose, we find the transpose matrix for each block and just check if all entries are same. If yes, then matrix is symmetric, else we print false.

Checks: We check that matrix should exist in the database.

Number of blocks to write: 0
Number of blocks to read: (dimension/M) \* (dimension/M)

## COMMAND: COMPUTE

Again, similar to tranpose, we find the transpose for each block and get their difference. After that, similar to load function, we write them into the result file. 

Checks: We check that matrix should exist in the database and no matrix with the result name should exist.

Number of blocks to write: (dimension/M) \* (dimension/M)
Number of blocks to read: (dimension/M) \* (dimension/M)

>## LEARNINGS
>1. Efficient Data Organization: The importance of good data organization strategy and page design. This was specifically useful for the optimisation of transpose-related tasks, so we divided **NxN** matrix into smaller matrices, as mentioned earlier.
We also made sure that the memory was being used efficiently under the given constraints and use cases.
>2. Understanding the existing codebase: Understanding how exactly the table is being implemented was really important so as to think of how we can do similar operations for matrices and error handling.
>3. Importance of OOPS: Since the codebase was very modular and used OOPS principles, we got familiar with the codebase very quickly and could work on different parts of the code together without much issue. We tried to implement these principles wherever we could in our code.
>4. Team Collaboration: Since the tasks were very closely linked to each other, proper team collaboration was the key to completing this phase on time. Through proper discussion and brainstorming, we understood the initial code, came up with the optimal strategy to store the matrix, figured out what all changes had to be made, and came up with implementation logic and details for all tasks. This significantly reduced any errors, that would have been made if these tasks were done individually.
>5. Code reviews and testing: Reviewing the codes of other team members and testing their parts made sure that none of the edge cases were missed.

> ## CONTRIBUTIONS
> 
> We all collaborated and came up with the design and implementation logic for all the parts together. Any issues with the logic were resolved collaboratively through discussion among teammates.
Changes in buffermanager, page file, and blockify function were also made together since these were the backbone for this phase.
For the coding part, this was the division of work:
>>ARYAN - load and transpose
>
>>PRATHAM - print, rename and checksymmetry
>
>>ANMOL - export and compute
>
>(implementation details for these were discussed among teammates, so as to reduce errors and find the best approach).
