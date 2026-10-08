#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

const int ITERATIONS = 50000000;
mutex mtx;
int sharedVariable = 0;

void incrementWithMutex() {
    for (int i = 0; i < ITERATIONS; i++) {
        lock_guard< mutex> lock(mtx);
        sharedVariable++;
    }
}
int main() {
    auto start = chrono::high_resolution_clock::now();
    thread thread1(incrementWithMutex);
    thread thread2(incrementWithMutex);
    thread1.join();
    thread2.join();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "sharedVariable value with mutex: " << sharedVariable << endl;
    cout << "Execution time with mutex: " << elapsed.count() << " seconds" << endl;
    return 0;
}
