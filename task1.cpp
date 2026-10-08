#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <mutex>

using namespace std;

const int n = 10;
const int m = 10;
const int k = 10;
const int MAX_THREADS = n * k; 

int matrixA[n][m];
int matrixB[m][k];
int result[n][k];

mutex outputMutex;

void fillMatrix(int matrix[][k], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void multiplyCell(int row, int col) {
    result[row][col] = 0;
    for (int i = 0; i < m; i++) {
        result[row][col] += matrixA[row][i] * matrixB[i][col];
    }

    {
        lock_guard<std::mutex> guard(outputMutex);
        cout << "[" << row << "," << col << "]=" << result[row][col] << endl;
    }
}

int main() {
    srand(time(nullptr));
    
    fillMatrix(matrixA, n, m);
    fillMatrix(matrixB, m, k);
    
    for (int numThreads = 1; numThreads <= MAX_THREADS; numThreads += 1) {
        vector< thread> threads;
        auto start = chrono::high_resolution_clock::now();
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                threads.emplace_back(multiplyCell, i, j);
                
                if (threads.size() >= numThreads) {
                    for (auto& thread : threads) {
                        thread.join();
                    }
                    threads.clear();
                }
            }
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "Execution time for " << numThreads << " threads: " << elapsed.count() << " seconds" << endl;
    }
    
    return 0;
}
