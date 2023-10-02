// Watch out for memory limit
// Sparse table's build and memory complexity are
// O(N * M * log2(N) * log2(M))
// Check for when M = N
// Don't forget that you need to call the read and build function!
struct SparseTable2D {
    int LGN = 10, LGM = 10;
    vector<int> lg1, lg2;
    vector<vector<int>> a;
    int N, M;

    int* st_internal;

    inline int& st(int x, int y, int a, int b) {
        return st_internal[x * M * LGN * LGM + y * LGN * LGM + a * LGM + b];
    }

    SparseTable2D(int n, int m) {
        N = n;
        M = m;

        LGN = log2(N) + 1;
        LGM = log2(M) + 1;
        
        lg1.resize(N + 1);
        lg2.resize(M + 1);

        st_internal = new int[N * M * LGN * LGM + 10];
        memset(st_internal, INT_MAX, sizeof st_internal);
        // Don't forget to change INT_MAX if it's not a MIN query.
    }

    ~SparseTable2D() {
        delete[] st_internal;
    }

    void read() {
        a.resize(N, vector<int>(M, 0));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cin >> a[i][j];
            }
        }
    }

    void build() {
        for (int i = 2; i <= N; i++) lg1[i] = lg1[i >> 1] + 1;
        for (int i = 2; i <= M; i++) lg2[i] = lg2[i >> 1] + 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                st(i, j, 0, 0) = a[i][j];
            }
        }
        for (int a = 0; a < LGN; a++) {
            for (int b = 0; b < LGM; b++) {
                if (a + b == 0) continue;
                for (int i = 0; i + (1 << a) <= N; i++) {
                    for (int j = 0; j + (1 << b) <= M; j++) {
                        if (!a) {
                            st(i, j, a, b) = min(
                                st(i, j, a, b - 1),
                                st(i, j + (1 << (b - 1)), a, b - 1));
                        } else {
                            st(i, j, a, b) = min(
                                st(i, j, a - 1, b),
                                st(i + (1 << (a - 1)), j, a - 1, b));
                        }
                    }
                }
            }
        }
    }

    int query(int x1, int y1, int x2, int y2) {
        x2++; y2++;
        int a = lg1[x2 - x1], b = lg2[y2 - y1];
        return min(
                min(st(x1, y1, a, b), st(x2 - (1 << a), y1, a, b)),
                min(st(x1, y2 - (1 << b), a, b), st(x2 - (1 << a), y2 - (1 << b), a, b))
            );
    };
};