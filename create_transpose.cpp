#include <fstream>
#include <iostream>
#include <vector>

int main() {
    const int numRows = 100;
    const int numCols = 100;

    // Read the matrix from the "test.csv" file
    std::ifstream inputFile("./data/original.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the input CSV file." << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> matrix(numRows, std::vector<int>(numCols));

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            char comma;
            inputFile >> matrix[row][col];

            // Read the comma (or check for EOF)
            if (col < numCols - 1) {
                if (!(inputFile >> comma) || comma != ',') {
                    std::cerr << "Error reading from the input CSV file." << std::endl;
                    inputFile.close();
                    return 1;
                }
            }
        }
    }

    inputFile.close();

    // Find the transpose of the matrix
    std::vector<std::vector<int>> transpose(numCols, std::vector<int>(numRows));

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            transpose[col][row] = matrix[row][col];
        }
    }

    // Write the transpose to a different file, e.g., "transpose.csv"
    std::ofstream outputFile("./data/compute.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the output CSV file." << std::endl;
        return 1;
    }

    for (int row = 0; row < numCols; ++row) {
        for (int col = 0; col < numRows; ++col) {
            outputFile << matrix[row][col] - transpose[row][col];

            // Add a comma after each value except the last one in a row
            if (col < numRows - 1) {
                outputFile << ",";
            }
        }
        // Add a newline character after each row
        outputFile << "\n";
    }

    outputFile.close();
    std::cout << "Transpose matrix written to 'transpose.csv' successfully." << std::endl;

    return 0;
}
