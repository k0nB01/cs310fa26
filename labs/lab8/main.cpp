#include "sortedType.h"
#include <iostream>
using namespace std;

// Print one complete trip around the circular list.
template <class T>
void PrintList(SortedType<T>& list)
{
    list.ResetList();
    for (int i = 0; i < list.GetLength(); i++)
        cout << list.GetNextItem() << ' ';
    cout << endl;
}

// Use the same simple tests for each data type.
template <class T>
void TestList(T first, T second, T third, T missing)
{
    SortedType<T> list;
    bool found;
    T item;

    cout << "Empty length: " << list.GetLength() << " (expected 0)" << endl;
    cout << "IsFull: " << list.IsFull() << " (expected 0)" << endl;

    list.PutItem(second);
    list.PutItem(first);
    list.PutItem(third);
    cout << "Length after inserts: " << list.GetLength() << " (expected 3)" << endl;
    cout << "Forward: ";
    PrintList(list);
    cout << "Expected: " << first << ' ' << second << ' ' << third << endl;
    cout << "Reverse: ";
    list.PrintReverse();
    cout << "Expected: " << third << ' ' << second << ' ' << first << endl;

    item = second;
    list.GetItem(item, found);
    cout << "Find existing item: " << found << " (expected 1)" << endl;
    item = missing;
    list.GetItem(item, found);
    cout << "Find missing item: " << found << " (expected 0)" << endl;

    list.ResetList();
    cout << "After ResetList: " << list.GetNextItem() << " (expected " << first << ")" << endl;
    list.GetNextItem();
    list.GetNextItem();
    cout << "Wrap to first: " << list.GetNextItem() << " (expected " << first << ")" << endl;

    list.PutItem(second);
    list.DeleteItem(second);
    cout << "Insert duplicate, delete one: ";
    PrintList(list);
    cout << "Expected: " << first << ' ' << second << ' ' << third << endl;
    list.DeleteItem(missing);
    cout << "Delete missing, length: " << list.GetLength() << " (expected 3)" << endl;
    list.DeleteItem(second);
    cout << "Delete middle: ";
    PrintList(list);
    cout << "Expected: " << first << ' ' << third << endl;
    list.DeleteItem(third);
    cout << "Delete tail: ";
    PrintList(list);
    cout << "Expected: " << first << endl;
    list.PutItem(third);
    list.DeleteItem(first);
    cout << "Delete head: ";
    PrintList(list);
    cout << "Expected: " << third << endl;
    list.DeleteItem(third);
    cout << "Delete only node, length: " << list.GetLength() << " (expected 0)" << endl;

    list.PutItem(first);
    list.PutItem(second);
    list.MakeEmpty();
    cout << "MakeEmpty length: " << list.GetLength() << " (expected 0)" << endl;
    cout << "Empty reverse (blank line):" << endl;
    list.PrintReverse();
    list.DeleteItem(missing);
    list.PutItem(third);
    cout << "Reuse after clearing: ";
    PrintList(list);
    cout << "Expected: " << third << endl;
    // The destructor frees the remaining node when this function ends.
}

int main()
{
    cout << "=== int ===" << endl;
    TestList(10, 20, 30, 99);
    cout << "\n=== double ===" << endl;
    TestList(1.5, 2.5, 3.5, 9.5);
    cout << "\n=== char ===" << endl;
    TestList('X', 'Y', 'Z', 'A');
    return 0;
}
