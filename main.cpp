#include <iostream>
#include "LazyLinkedList.h"
using namespace std;

int main() {
    LazyLinkedList<int> list;

    list.insert(10);
    list.insert(20);
    list.insert(30);
    list.insert(40);
    list.insert(50);

    cout << "Initial list: ";
    list.print();
    cout << "Active nodes: " << list.size() << ", Hidden nodes: " << list.hiddenSize() << endl;

    list.remove(10);
    list.remove(30);

    cout << "\nAfter lazy deleting 10 and 30: ";
    list.print();
    cout << "Active nodes: " << list.size() << ", Hidden nodes: " << list.hiddenSize() << endl;

    list.remove(20);
    cout << "\nAfter lazy deleting 20: ";
    list.print();
    cout << "Active nodes: " << list.size() << ", Hidden nodes: " << list.hiddenSize() << endl;

    cout << "\nSearch for 40: " << (list.search(40) ? "Found" : "Not Found") << endl;
    cout << "Search for 10: " << (list.search(10) ? "Found" : "Not Found") << endl;

    return 0;
}
