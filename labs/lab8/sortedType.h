#ifndef SORTED_TYPE_H
#define SORTED_TYPE_H

#include <iostream>
#include <new>
#include <stdexcept>

// A sorted, singly linked circular list. ItemType must support < and ==.
template <class ItemType>
class SortedType
{
public:
    SortedType();
    ~SortedType();

    // This class owns its nodes; accidental shallow copying is forbidden.
    SortedType(const SortedType&) = delete;
    SortedType& operator=(const SortedType&) = delete;

    bool IsFull() const;
    int GetLength() const;
    void MakeEmpty();
    ItemType GetItem(ItemType& item, bool& found);
    void PutItem(ItemType item);
    void DeleteItem(ItemType item);
    void ResetList();
    ItemType GetNextItem();
    void PrintReverse() const;

private:
    struct NodeType
    {
        ItemType info;
        NodeType* next;
    };

    NodeType* listData; // Tail (largest item); tail->next is the head.
    int length;
    NodeType* currentPos;

    void FindItem(const ItemType& item, NodeType*& location,
                  NodeType*& predLoc, bool& found) const;
    void PrintReverseNodes(const NodeType* node, int remaining) const;
};

// Template definitions must be visible wherever the template is instantiated.
#include "sortedType.cpp"
#endif
