template<typename T>
vector<vector<T>> rotate(const vector<vector<T>>& matrix, int x, int y) {
    vector<vector<T>> result(x, vector<T>(y, 0));
    int newColumn, newRow = 0;
    for (int oldColumn = x - 1; oldColumn >= 0; oldColumn--) {
        newColumn = 0;
        for (int oldRow = 0; oldRow < y; oldRow++) {
            result[newRow][newColumn] = matrix[oldRow][oldColumn];
            newColumn++;
        }
        newRow++;
    }

    return result;
}

template<typename T>
vector<vector<T>> flipH(const vector<vector<T>> &matrix, int n) {
    vector<vector<T>> result(n, vector<T>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = matrix[i][n-j-1];
        }
    }

    return result;
}

template<typename T>
vector<vector<T>> flipV(const vector<vector<T>> &matrix, int n) {
    vector<vector<T>> result(n, vector<T>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = matrix[n-i-1][j];
        }
    }

    return result;
}
