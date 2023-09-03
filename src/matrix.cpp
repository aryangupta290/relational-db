#include <global.h>
#include <matrix.h>

/**
 * @brief Construct a new Matrix:: Matrix object
 *
 */
Matrix::Matrix() {
    logger.log("Matrix::Matrix");
}

/**
 * @brief Construct a new Matrix:: Matrix object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param matrixName
 */
Matrix::Matrix(string matrixName) {
    logger.log("Matrix::Matrix");
    this->sourceFileName = "../data/" + matrixName + ".csv";
    this->matrixName = matrixName;
    this->hashedName = "";
    for (int i = 0; i < 26; i++) {
        int c = rand() % 26;
        hashedName.push_back(c + 'a');
    }
    logger.log(hashedName);
}

bool Matrix::load() {
    logger.log("Matrix::load");
    ifstream f(this->sourceFileName, ios::in);
    string ff;
    getline(f, ff);
    int comma_count = 1;
    for (int i = 0; i < ff.size(); i++) {
        if (ff[i] == ',') {
            comma_count++;
        }
    }
    this->dimension = comma_count;
    int blocks = this->dimension / MATRIX_PAGE_DIMENSION;
    if (this->dimension % MATRIX_PAGE_DIMENSION) blocks++;
    this->blockCountSqrt = blocks;
    this->blockCount = blocks * blocks;
    logger.log(to_string(this->blockCountSqrt));
    logger.log(to_string(this->blockCount));
    logger.log(to_string(MATRIX_PAGE_DIMENSION));

    return this->blockify();
}

void print_info() {
    cout << "Number of blocks read: " << num_blocks_read << endl;
    cout << "Number of blocks written: " << num_blocks_write << endl;
    cout << "Number of blocks accessed: " << num_blocks_read + num_blocks_write << endl;
}

bool Matrix::blockify() {
    logger.log("Matrix::blockify");
    ifstream f(this->sourceFileName, ios::in);
    int readTillNow = 0;
    for (int blockx = 0; blockx < this->blockCountSqrt; blockx++) {
        int res[MATRIX_PAGE_DIMENSION][this->blockCountSqrt * MATRIX_PAGE_DIMENSION];
        memset(res, 0, sizeof(res));
        while (readTillNow < this->dimension && readTillNow < (1 + blockx) * MATRIX_PAGE_DIMENSION) {
            string line = "";
            getline(f, line);
            string num;
            stringstream s(line);
            for (int p = 0; p < this->dimension; p++) {
                getline(s, num, ',');
                while (num.size() && num.back() == ' ') {
                    num.pop_back();
                }
                reverse(num.begin(), num.end());
                while (num.size() && num.back() == ' ') {
                    num.pop_back();
                }
                reverse(num.begin(), num.end());
                res[readTillNow - blockx * MATRIX_PAGE_DIMENSION][p] = (stoi(num));
            }
            readTillNow++;
        }
        for (int blocky = 0; blocky < this->blockCountSqrt; blocky++) {
            vector<vector<int>> cur_page(MATRIX_PAGE_DIMENSION, vector<int>(MATRIX_PAGE_DIMENSION));
            for (int x = 0; x < MATRIX_PAGE_DIMENSION; x++) {
                for (int u = blocky * MATRIX_PAGE_DIMENSION; u < (blocky + 1) * MATRIX_PAGE_DIMENSION; u++) {
                    cur_page[x][u - blocky * MATRIX_PAGE_DIMENSION] = res[x][u];
                }
            }
            bufferManager.writePageForMatrix(this->hashedName, blockx, blocky, cur_page);
            num_blocks_write++;
        }
    }
    f.close();
    print_info();
    return true;
}

vector<vector<int>> transpose_help(vector<vector<int>> res) {
    vector<vector<int>> ans = res;
    for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < res.size(); j++) {
            ans[i][j] = res[j][i];
        }
    }
    return ans;
}

void Matrix::transpose() {
    logger.log("Matrix::transpose");
    for (int i = 0; i < this->blockCountSqrt; i++) {
        vector<vector<int>> res = bufferManager.getMatrixPage(this->hashedName, i, i).rows;
        num_blocks_read++;
        vector<vector<int>> transposedMatrix = transpose_help(res);
        bufferManager.writePageForMatrix(this->hashedName, i, i, transposedMatrix);
        num_blocks_write++;
        for (int j = i + 1; j < this->blockCountSqrt; j++) {
            vector<vector<int>> res1 = bufferManager.getMatrixPage(this->hashedName, i, j).rows;
            num_blocks_read++;
            vector<vector<int>> transposedMatrix1 = transpose_help(res1);
            vector<vector<int>> res2 = bufferManager.getMatrixPage(this->hashedName, j, i).rows;
            num_blocks_read++;
            vector<vector<int>> transposedMatrix2 = transpose_help(res2);
            bufferManager.writePageForMatrix(this->hashedName, i, j, transposedMatrix2);
            bufferManager.writePageForMatrix(this->hashedName, j, i, transposedMatrix1);
            num_blocks_write += 2;
        }
    }
    print_info();
    return;
}

bool check_equal(vector<vector<int>> comp1, vector<vector<int>> comp2) {
    for (int i = 0; i < MATRIX_PAGE_DIMENSION; i++) {
        for (int j = 0; j < MATRIX_PAGE_DIMENSION; j++) {
            if (comp1[i][j] != comp2[i][j]) return false;
        }
    }
    return true;
}

void subtract_matrix(vector<vector<int>> &a, vector<vector<int>> b) {
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a.size(); j++) {
            a[i][j] -= b[i][j];
        }
    }
}

void Matrix::compute() {
    logger.log("Matrix::compute");

    Matrix *matrix = new Matrix(this->matrixName + "_RESULT");
    matrix->dimension = this->dimension;
    matrix->blockCount = this->blockCount;
    matrix->blockCountSqrt = this->blockCountSqrt;
    for (int i = 0; i < blockCountSqrt; i++) {
        vector<vector<int>> res = bufferManager.getMatrixPage(this->hashedName, i, i).rows;
        num_blocks_read++;
        vector<vector<int>> transposedMatrix = transpose_help(res);
        subtract_matrix(res, transposedMatrix);
        bufferManager.writePageForMatrix(matrix->hashedName, i, i, res);
        num_blocks_write++;
        for (int j = i + 1; j < blockCountSqrt; j++) {
            vector<vector<int>> res1 = bufferManager.getMatrixPage(this->hashedName, i, j).rows;
            vector<vector<int>> transposedMatrix1 = transpose_help(res1);
            vector<vector<int>> res2 = bufferManager.getMatrixPage(this->hashedName, j, i).rows;
            vector<vector<int>> transposedMatrix2 = transpose_help(res2);
            subtract_matrix(res1, transposedMatrix2);
            subtract_matrix(res2, transposedMatrix1);
            bufferManager.writePageForMatrix(matrix->hashedName, i, j, res1);
            bufferManager.writePageForMatrix(matrix->hashedName, j, i, res2);
            num_blocks_read += 2;
            num_blocks_write += 2;
        }
    }
    matrixCatalogue.insertMatrix(matrix);
    print_info();
    return;
}

void Matrix::rename(string newFileName) {
    logger.log("Matrix::rename");
    matrixCatalogue.eraseMatrix(this->matrixName);
    this->matrixName = newFileName;
    matrixCatalogue.insertMatrix(this);
    return;
}

void Matrix::checksymmetry() {
    logger.log("Matrix::checksymmetry");
    for (int i = 0; i < this->blockCountSqrt; i++) {
        vector<vector<int>> res = bufferManager.getMatrixPage(this->hashedName, i, i).rows;
        num_blocks_read++;
        vector<vector<int>> transposedMatrix = transpose_help(res);
        if (!check_equal(res, transposedMatrix)) {
            cout << "FALSE\n";
            print_info();
            return;
        }
        for (int j = i + 1; j < this->blockCountSqrt; j++) {
            vector<vector<int>> res1 = bufferManager.getMatrixPage(this->hashedName, i, j).rows;
            vector<vector<int>> transposedMatrix1 = transpose_help(res1);
            vector<vector<int>> res2 = bufferManager.getMatrixPage(this->hashedName, j, i).rows;
            vector<vector<int>> transposedMatrix2 = transpose_help(res2);
            num_blocks_read += 2;
            if (!check_equal(res1, transposedMatrix2) || !check_equal(res2, transposedMatrix1)) {
                cout << "FALSE\n";
                print_info();
                return;
            }
        }
    }
    cout << "TRUE\n";
    print_info();
    return;
}

void Matrix::export_mat() {
    logger.log("Matrix::export");
    string fileName = "../data/" + matrixName + ".csv";
    ofstream f(fileName, ios::trunc);
    int rcnt = 0;
    for (int i = 0; i < this->blockCountSqrt; i++) {
        int store[MATRIX_PAGE_DIMENSION][this->blockCountSqrt * MATRIX_PAGE_DIMENSION];
        for (int j = 0; j < this->blockCountSqrt; j++) {
            vector<vector<int>> res = bufferManager.getMatrixPage(this->hashedName, i, j).rows;
            num_blocks_read++;
            for (int p1 = 0; p1 < MATRIX_PAGE_DIMENSION; p1++) {
                for (int p2 = j * MATRIX_PAGE_DIMENSION; p2 < (j + 1) * MATRIX_PAGE_DIMENSION; p2++) {
                    store[p1][p2] = res[p1][p2 - j * MATRIX_PAGE_DIMENSION];
                }
            }
        }
        for (int p = 0; p < MATRIX_PAGE_DIMENSION; p++) {
            if (rcnt == this->dimension) break;
            rcnt++;
            f << store[p][0];
            for (int j = 1; j < this->dimension; j++) {
                f << "," << store[p][j];
            }
            f << endl;
        }
    }
    print_info();
    f.close();
}

void Matrix::print() {
    logger.log("Matrix::print");
    long long int ans[41][41];
    for (int i = 0; i < 41; i++) {
        for (int j = 0; j < 41; j++) ans[i][j] = (long long int)1e18;
    }
    int dx[] = {0, 0, 1, 1};
    int dy[] = {0, 1, 0, 1};
    for (int p = 0; p < 4; p++) {
        int blockx = dx[p];
        int blocky = dy[p];
        if (blockx >= this->blockCountSqrt || blocky >= this->blockCountSqrt) continue;
        vector<vector<int>> res = bufferManager.getMatrixPage(this->hashedName, blockx, blocky).rows;
        num_blocks_read++;
        int ii = 0, jj = 0;
        for (int r = blockx * MATRIX_PAGE_DIMENSION; r < (blockx + 1) * MATRIX_PAGE_DIMENSION; r++) {
            jj = 0;
            for (int c = blocky * MATRIX_PAGE_DIMENSION; c < (blocky + 1) * MATRIX_PAGE_DIMENSION; c++) {
                ans[r][c] = res[ii][jj];
                jj++;
            }
            ii++;
        }
    }
    for (int i = 0; i < min(20, (int)this->dimension); i++) {
        for (int j = 0; j < min(20, (int)this->dimension); j++) {
            cout << ans[i][j] << " ";
        }
        cout << endl;
    }
    print_info();
    return;
}

void Matrix::unload() {
    logger.log("Matrix::_unload");
    for (int row = 0; row < this->blockCountSqrt; row++) {
        for (int col = 0; col < this->blockCountSqrt; col++) {
            bufferManager.deletePageForMatrix(this->hashedName, row, col);
        }
    }
}