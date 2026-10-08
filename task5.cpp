#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

using namespace std;

const int ITERATIONS = 1000;
atomic<int> sharedVariable(0);
mutex mtx;

void incrementSynchronously(int iterations, int threadId) {
    while (sharedVariable.load() < iterations) {
        int current = sharedVariable.load();
        if ((current % 2 == threadId) && (current < iterations)) {
            int local = sharedVariable.fetch_add(1);
            if (local + 1 <= iterations) {
                lock_guard<mutex> lock(mtx);
                cout << local + 1 << "\n";
            }
        }
    }
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    thread thread1(incrementSynchronously, ITERATIONS, 0);
    thread thread2(incrementSynchronously, ITERATIONS, 1);

    thread1.join();
    thread2.join(); 

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "sharedVariable value: " << sharedVariable.load() << endl;
    cout << "Execution time: " << elapsed.count() << " seconds" << endl;

    return 0;
}
