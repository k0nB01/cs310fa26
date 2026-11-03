#ifndef SORTED_TYPE_IMPLEMENTATION
#define SORTED_TYPE_IMPLEMENTATION
#include "sortedType.h"

template <class ItemType>
SortedType<ItemType>::SortedType()
    : listData(nullptr), length(0), currentPos(nullptr)
{
}

template <class ItemType>
SortedType<ItemType>::~SortedType()
{
    MakeEmpty();
}

template <class ItemType>
bool SortedType<ItemType>::IsFull() const
{
    try
    {
        NodeType* location = new NodeType;
        delete location;
        return false;
    }
    catch (const std::bad_alloc&)
    {
        return true;
    }
}

template <class ItemType>
int SortedType<ItemType>::GetLength() const
{
    return length;
}

template <class ItemType>
void SortedType<ItemType>::MakeEmpty()
{
    if (listData != nullptr)
    {
        NodeType* location = listData->next;
        listData->next = nullptr; // Break the circle before freeing nodes.
        while (location != nullptr)
        {
            NodeType* temp = location;
            location = location->next;
            delete temp;
        }
    }
    listData = nullptr;
    length = 0;
    ResetList();
}

// Like circle.cpp: start at the head with the tail as its predecessor.
// Precondition: the list is nonempty. Stop after at most one full circle.
template <class ItemType>
void SortedType<ItemType>::FindItem(const ItemType& item, NodeType*& location,
                                    NodeType*& predLoc, bool& found) const
{
    location = listData->next;
    predLoc = listData;
    found = false;
    do
    {
        if (item < location->info)
            return;
        if (item == location->info)
        {
            found = true;
            return;
        }
        predLoc = location;
        location = location->next;
    } while (location != listData->next);
}

template <class ItemType>
ItemType SortedType<ItemType>::GetItem(ItemType& item, bool& found)
{
    found = false;
    if (listData != nullptr)
    {
        NodeType* location;
        NodeType* predLoc;
        FindItem(item, location, predLoc, found);
        if (found)
            item = location->info;
    }
    return item;
}

template <class ItemType>
void SortedType<ItemType>::PutItem(ItemType item)
{
    if (listData == nullptr)
    {
        listData = new NodeType{item, nullptr};
        listData->next = listData;
    }
    else
    {
        NodeType* location;
        NodeType* predLoc;
        bool found;
        FindItem(item, location, predLoc, found);
        const bool newLargest = listData->info < item;
        NodeType* newNode = new NodeType{item, location};
        predLoc->next = newNode;
        if (newLargest)
            listData = newNode;
        // Equal items are inserted before the first matching item.
    }
    ++length;
    ResetList();
}

template <class ItemType>
void SortedType<ItemType>::DeleteItem(ItemType item)
{
    if (listData == nullptr)
        return;
    NodeType* location;
    NodeType* predLoc;
    bool found;
    FindItem(item, location, predLoc, found);
    if (!found)
        return;

    if (location == predLoc) // Only node: it points to itself.
        listData = nullptr;
    else
    {
        predLoc->next = location->next;
        if (location == listData)
            listData = predLoc;
    }
    delete location;
    --length;
    ResetList();
}

template <class ItemType>
void SortedType<ItemType>::ResetList()
{
    currentPos = nullptr;
}

template <class ItemType>
ItemType SortedType<ItemType>::GetNextItem()
{
    if (listData == nullptr)
        throw std::out_of_range("GetNextItem called on an empty list");
    if (currentPos == nullptr)
        currentPos = listData->next;
    ItemType item = currentPos->info;
    currentPos = currentPos->next;
    return item; // Traversal naturally wraps from tail to head.
}

template <class ItemType>
void SortedType<ItemType>::PrintReverseNodes(const NodeType* node,
                                            int remaining) const
{
    if (remaining == 0)
        return;
    PrintReverseNodes(node->next, remaining - 1);
    if (remaining > 1)
        std::cout << ' ';
    std::cout << node->info;
}

template <class ItemType>
void SortedType<ItemType>::PrintReverse() const
{
    if (listData != nullptr)
        PrintReverseNodes(listData->next, length);
    std::cout << '\n';
}
#endif
