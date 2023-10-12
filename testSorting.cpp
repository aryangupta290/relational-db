#include <fstream>
#include <iostream>

int main() {
    const int numRows = 200;
    const int numCols = 5;
    const int maxValue = 100;

    std::ofstream csvFile("./data/testSort.csv");

    if (!csvFile.is_open()) {
        std::cerr << "Error opening the CSV file." << std::endl;
        return 1;
    }
    csvFile << "a, b, c, d, e\n";
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            int value = rand() % maxValue + 1;
            csvFile << value;

            // Add a comma after each value except the last one in a row
            if (col < numCols - 1) {
                csvFile << ", ";
            }
        }
        // Add a newline character after each row
        csvFile << "\n";
    }

    csvFile.close();
    std::cout << "CSV file 'test.csv' created successfully." << std::endl;

    return 0;
}
