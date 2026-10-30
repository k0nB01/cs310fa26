#ifndef LAB61_STRUCTURES_H
#define LAB61_STRUCTURES_H

#include <iostream>
#include <limits>
#include <stdexcept>

// Array stack: start with 100 spaces and double when full.
template <typename T>
class Stack {
    T* arr;
    int capacity;
    int count;
public:
    Stack() : arr(new T[100]), capacity(100), count(0) {}
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
    ~Stack() { delete[] arr; }
    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == capacity; }
    int size() const { return count; }
    void push(T item) {
        if (isFull()) {
            if (capacity > std::numeric_limits<int>::max() / 2)
                throw std::overflow_error("Stack capacity overflow");
            T* bigger = new T[capacity * 2];
            try {
                for (int i = 0; i < count; ++i) bigger[i] = arr[i];
            } catch (...) {
                delete[] bigger;
                throw;
            }
            delete[] arr;
            arr = bigger;
            capacity *= 2;
        }
        arr[count] = item;
        ++count;
    }
    T pop() {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        T item = arr[count - 1];
        --count;
        return item;
    }
    T top() const {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return arr[count - 1];
    }
};

// The top is rear->next. The rear points back to the top.
template <typename T>
class CircularLinkedStack {
    struct Node {
        T data;
        Node* next;
        Node(T item) : data(item), next(nullptr) {}
    };
    Node* rear;
    int count;
public:
    CircularLinkedStack() : rear(nullptr), count(0) {}
    CircularLinkedStack(const CircularLinkedStack&) = delete;
    CircularLinkedStack& operator=(const CircularLinkedStack&) = delete;
    ~CircularLinkedStack() {
        while (rear) {
            Node* old = rear->next;
            if (old == rear) rear = nullptr;
            else rear->next = old->next;
            delete old;
        }
    }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void push(T item) {
        Node* node = new Node(item);
        if (isEmpty()) {
            rear = node;
            node->next = node;
        } else {
            node->next = rear->next;
            rear->next = node;
        }
        ++count;
    }
    T pop() {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        Node* old = rear->next;
        T item = old->data;
        if (old == rear) rear = nullptr;
        else rear->next = old->next;
        delete old;
        --count;
        return item;
    }
    T top() const {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return rear->next->data;
    }
    void display(std::ostream& out = std::cout) const {
        Stack<T> reverse;
        if (rear) {
            Node* current = rear->next;
            for (int i = 0; i < count; ++i) {
                reverse.push(current->data);
                current = current->next;
            }
        }
        while (!reverse.isEmpty()) out << reverse.pop();
    }
};

// Fixed-capacity circular array queue, as required by the lab.
template <typename T>
class Queue {
    T* arr;
    int capacity;
    int frontIndex;
    int rearIndex;
    int count;
public:
    Queue() : arr(new T[100]), capacity(100), frontIndex(0),
              rearIndex(-1), count(0) {}
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    ~Queue() { delete[] arr; }
    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == capacity; }
    int size() const { return count; }
    void enqueue(T item) {
        if (isFull()) throw std::overflow_error("Queue is full");
        int next = (rearIndex + 1) % capacity;
        arr[next] = item;
        rearIndex = next;
        ++count;
    }
    T dequeue() {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        T item = arr[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        --count;
        return item;
    }
    T front() const {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        return arr[frontIndex];
    }
};

template <typename T>
class CircularLinkedQueue {
    struct Node {
        T data;
        Node* next;
        Node(T item) : data(item), next(nullptr) {}
    };
    Node* rear;
    int count;
public:
    CircularLinkedQueue() : rear(nullptr), count(0) {}
    CircularLinkedQueue(const CircularLinkedQueue&) = delete;
    CircularLinkedQueue& operator=(const CircularLinkedQueue&) = delete;
    ~CircularLinkedQueue() {
        while (rear) {
            Node* old = rear->next;
            if (old == rear) rear = nullptr;
            else rear->next = old->next;
            delete old;
        }
    }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void enqueue(T item) {
        Node* node = new Node(item);
        if (isEmpty()) node->next = node;
        else {
            node->next = rear->next;
            rear->next = node;
        }
        rear = node;
        ++count;
    }
    T dequeue() {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        Node* old = rear->next;
        T item = old->data;
        if (old == rear) rear = nullptr;
        else rear->next = old->next;
        delete old;
        --count;
        return item;
    }
    T front() const {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        return rear->next->data;
    }
};

template <typename T>
class SinglyLinkedList {
    struct Node {
        T data;
        Node* next;
        Node(T item) : data(item), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int count;
public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;
    ~SinglyLinkedList() { while (!isEmpty()) deleteFront(); }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void insertFront(T item) {
        Node* node = new Node(item);
        node->next = head;
        head = node;
        if (!tail) tail = node;
        ++count;
    }
    void insertRear(T item) {
        Node* node = new Node(item);
        if (tail) tail->next = node;
        else head = node;
        tail = node;
        ++count;
    }
    void deleteFront() {
        if (isEmpty()) throw std::underflow_error("List is empty");
        Node* old = head;
        head = head->next;
        delete old;
        if (!head) tail = nullptr;
        --count;
    }
    void deleteRear() {
        if (isEmpty()) throw std::underflow_error("List is empty");
        if (head == tail) { deleteFront(); return; }
        Node* current = head;
        while (current->next != tail) current = current->next;
        delete tail;
        tail = current;
        tail->next = nullptr;
        --count;
    }
    void display(std::ostream& out = std::cout) const {
        for (Node* current = head; current; current = current->next)
            out << current->data << ' ';
        out << '\n';
    }
};

template <typename T>
class CircularLinkedList {
    struct Node {
        T data;
        Node* next;
        Node(T item) : data(item), next(nullptr) {}
    };
    Node* rear;
    int count;
public:
    CircularLinkedList() : rear(nullptr), count(0) {}
    CircularLinkedList(const CircularLinkedList&) = delete;
    CircularLinkedList& operator=(const CircularLinkedList&) = delete;
    ~CircularLinkedList() { while (!isEmpty()) deleteFront(); }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void insertFront(T item) {
        Node* node = new Node(item);
        if (isEmpty()) {
            rear = node;
            node->next = node;
        } else {
            node->next = rear->next;
            rear->next = node;
        }
        ++count;
    }
    void insertRear(T item) {
        insertFront(item);
        rear = rear->next;
    }
    void deleteFront() {
        if (isEmpty()) throw std::underflow_error("List is empty");
        Node* old = rear->next;
        if (old == rear) rear = nullptr;
        else rear->next = old->next;
        delete old;
        --count;
    }
    void deleteRear() {
        if (isEmpty()) throw std::underflow_error("List is empty");
        if (count == 1) { deleteFront(); return; }
        Node* current = rear->next;
        while (current->next != rear) current = current->next;
        current->next = rear->next;
        delete rear;
        rear = current;
        --count;
    }
    void display(std::ostream& out = std::cout) const {
        if (rear) {
            Node* current = rear->next;
            for (int i = 0; i < count; ++i) {
                out << current->data << ' ';
                current = current->next;
            }
        }
        out << '\n';
    }
};

template <typename T>
class TextEditorBuffer {
    CircularLinkedStack<T> buffer;
    CircularLinkedQueue<T> redoQueue;
public:
    void type(T item) {
        buffer.push(item);
        // A new edit clears the old redo path.
        while (!redoQueue.isEmpty()) redoQueue.dequeue();
    }
    void undo() {
        if (buffer.isEmpty())
            throw std::underflow_error("Nothing to undo");
        int oldCount = redoQueue.size();
        redoQueue.enqueue(buffer.top());
        buffer.pop();
        // Move older undone items behind the newest undone item.
        for (int i = 0; i < oldCount; ++i)
            redoQueue.enqueue(redoQueue.dequeue());
    }
    void redo() {
        if (redoQueue.isEmpty())
            throw std::underflow_error("Nothing to redo");
        buffer.push(redoQueue.front());
        redoQueue.dequeue();
    }
    void displayBuffer(std::ostream& out = std::cout) const {
        buffer.display(out);
        out << '\n';
    }
};

#endif

