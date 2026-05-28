#include <iostream>
#include <chrono>
using namespace std;

void moveDisks(int, int, int, int);
void printIt(int , int , int );

int main() {
    for (auto numdisks: {5, 10, 15, 20, 25, 30, 35}) {
        auto start = chrono::steady_clock::now();
        moveDisks(numdisks, 1, 3, 2);
        auto end = chrono::steady_clock::now();

        chrono::duration<double> elapsed = end - start;
        cout << "numdisks: " << numdisks
             << " time passed: " << scientific << elapsed.count() << "s" << endl;
    }
}

void moveDisks(int num, int fromPeg, int toPeg, int tempPeg) {
    if (num > 0) {
        moveDisks(num-1, fromPeg, tempPeg, toPeg);
        printIt(num, fromPeg, toPeg);
        moveDisks(num-1, tempPeg, toPeg, fromPeg);
    }
}

void printIt(int disk, int fromPeg, int toPeg) {
    
}