#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

const int ITERATIONS = 50000000;
int sharedVariable = 0;

void incrementNoProtection() {
    for (int i = 0; i < ITERATIONS; i++) {
        sharedVariable++;
    }
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    thread thread1(incrementNoProtection);
    thread thread2(incrementNoProtection);

    thread1.join();
    thread2.join();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "sharedVariable value without protection: " << sharedVariable << endl;
    cout << "Execution time without protection: " << elapsed.count() << " seconds" << endl;

    return 0;
}
