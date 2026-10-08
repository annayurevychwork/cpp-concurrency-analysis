#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

const int ITERATIONS = 50000000;
atomic<int> sharedVariable(0);

void incrementWithLocalVariables() {
    int localCount = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        localCount++;
    }
    sharedVariable.fetch_add(localCount, memory_order_relaxed);
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    thread thread1(incrementWithLocalVariables);
    thread thread2(incrementWithLocalVariables);
    thread1.join();
    thread2.join();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "sharedVariable value with local variables: " << sharedVariable << std::endl;
    cout << "Execution time with local variables: " << elapsed.count() << " seconds" << endl;
    return 0;
}
