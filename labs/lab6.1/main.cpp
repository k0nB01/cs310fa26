#include "structures.h"
#include <sstream>
#include <string>

int passed = 0;
int failed = 0;

void check(bool condition, const std::string& name) {
    std::cout << (condition ? "PASS: " : "FAIL: ") << name << '\n';
    if (condition) ++passed;
    else ++failed;
}

template <typename Exception, typename Action>
void checkThrows(Action action, const std::string& name) {
    bool caught = false;
    try { action(); }
    catch (const Exception&) { caught = true; }
    catch (...) {}
    check(caught, name);
}

template <typename S>
void testStack(const std::string& name) {
    std::cout << "\n" << name << '\n';
    S stack;
    for (int i = 1; i <= 5; ++i) stack.push(i);
    bool order = stack.pop() == 5;
    order = (stack.pop() == 4) && order;
    order = (stack.pop() == 3) && order;
    std::cout << "Push: 1 2 3 4 5; pop: 5 4 3\n";
    std::cout << "Top: " << stack.top() << "; size: " << stack.size() << '\n';
    check(order && stack.top() == 2 && stack.size() == 2,
          "LIFO order, top and size");
    stack.pop(); stack.pop();
    checkThrows<std::underflow_error>([&]() { stack.pop(); }, "empty pop");
    checkThrows<std::underflow_error>([&]() { stack.top(); }, "empty top");
    stack.push(9);
    check(stack.pop() == 9 && stack.isEmpty(), "reuse after empty");
}

void testStacks() {
    testStack<Stack<int>>("ARRAY STACK");
    testStack<CircularLinkedStack<int>>("CIRCULAR LINKED STACK");
    Stack<int> stack;
    for (int i = 0; i < 100; ++i) stack.push(i);
    check(stack.isFull(), "initial capacity is 100");
    for (int i = 100; i < 250; ++i) stack.push(i);
    bool order = stack.size() == 250 && !stack.isFull();
    for (int i = 249; i >= 0; --i) order = (stack.pop() == i) && order;
    check(order && stack.isEmpty(), "growth to 250 items preserves order");
    Stack<std::string> words;
    words.push("hello");
    check(words.pop() == "hello", "array stack accepts strings");
}

template <typename Q>
void testQueue(const std::string& name) {
    std::cout << "\n" << name << '\n';
    Q queue;
    for (int i = 1; i <= 5; ++i) queue.enqueue(i);
    bool order = queue.dequeue() == 1;
    order = (queue.dequeue() == 2) && order;
    order = (queue.dequeue() == 3) && order;
    std::cout << "Enqueue: 1 2 3 4 5; dequeue: 1 2 3\n";
    std::cout << "Front: " << queue.front() << "; size: " << queue.size() << '\n';
    check(order && queue.front() == 4 && queue.size() == 2,
          "FIFO order, front and size");
    queue.dequeue(); queue.dequeue();
    checkThrows<std::underflow_error>([&]() { queue.dequeue(); }, "empty dequeue");
    checkThrows<std::underflow_error>([&]() { queue.front(); }, "empty front");
    queue.enqueue(9);
    check(queue.dequeue() == 9 && queue.isEmpty(), "reuse after empty");
}

void testQueues() {
    testQueue<Queue<int>>("ARRAY QUEUE");
    testQueue<CircularLinkedQueue<int>>("CIRCULAR LINKED QUEUE");
    Queue<int> queue;
    for (int i = 0; i < 100; ++i) queue.enqueue(i);
    check(queue.isFull(), "queue full at 100 items");
    checkThrows<std::overflow_error>([&]() { queue.enqueue(100); }, "queue overflow");
    bool order = queue.size() == 100 && queue.front() == 0;
    for (int i = 0; i < 60; ++i) order = (queue.dequeue() == i) && order;
    for (int i = 100; i < 160; ++i) queue.enqueue(i);
    for (int i = 60; i < 160; ++i) order = (queue.dequeue() == i) && order;
    check(order && queue.isEmpty(), "circular wraparound preserves FIFO order");
    Queue<std::string> words;
    words.enqueue("hello");
    check(words.dequeue() == "hello", "array queue accepts strings");
}

template <typename L>
void testList(const std::string& name) {
    std::cout << "\n" << name << '\n';
    L list;
    list.insertRear(3); list.insertFront(2); list.insertFront(1);
    list.insertRear(4); list.insertRear(5);
    std::cout << "After five inserts: "; list.display();
    std::ostringstream before;
    list.display(before);
    check(before.str() == "1 2 3 4 5 \n" && list.size() == 5,
          "front and rear insertion");
    list.deleteFront(); list.deleteRear();
    std::cout << "After deleting front and rear: "; list.display();
    std::ostringstream after;
    list.display(after);
    check(after.str() == "2 3 4 \n" && list.size() == 3,
          "front and rear deletion");
    list.deleteRear(); list.deleteFront(); list.deleteRear();
    check(list.isEmpty(), "delete final node from rear");
    checkThrows<std::underflow_error>([&]() { list.deleteFront(); }, "empty deleteFront");
    checkThrows<std::underflow_error>([&]() { list.deleteRear(); }, "empty deleteRear");
    list.insertFront(9); list.deleteFront();
    check(list.isEmpty(), "reuse and delete final node from front");
    std::ostringstream empty;
    list.display(empty);
    check(empty.str() == "\n", "empty display");
}

void testLists() {
    testList<SinglyLinkedList<int>>("SINGLY LINKED LIST");
    testList<CircularLinkedList<int>>("CIRCULAR LINKED LIST");
    CircularLinkedList<std::string> words;
    words.insertRear("hello"); words.insertRear("world");
    std::ostringstream out;
    words.display(out);
    check(out.str() == "hello world \n", "circular list accepts strings");
}

template <typename T>
std::string contents(const TextEditorBuffer<T>& editor) {
    std::ostringstream out;
    editor.displayBuffer(out);
    return out.str();
}

void testEditor() {
    std::cout << "\nTEXT EDITOR: CHARACTERS\n";
    TextEditorBuffer<char> editor;
    checkThrows<std::underflow_error>([&]() { editor.undo(); }, "empty undo");
    checkThrows<std::underflow_error>([&]() { editor.redo(); }, "empty redo");
    // Exactly ten characters; "hello world" would be eleven.
    for (char ch : std::string("helloworld")) editor.type(ch);
    std::cout << "Typed ten characters: "; editor.displayBuffer();
    check(contents(editor) == "helloworld\n", "ten-character input");
    editor.undo(); editor.undo(); editor.undo();
    std::cout << "Undo three: "; editor.displayBuffer();
    check(contents(editor) == "hellowo\n", "undo three characters");
    editor.redo(); editor.redo();
    std::cout << "Redo two: "; editor.displayBuffer();
    check(contents(editor) == "helloworl\n", "redo newest undone item first");
    editor.redo();
    check(contents(editor) == "helloworld\n", "restore original text");
    editor.undo(); editor.type('!');
    checkThrows<std::underflow_error>([&]() { editor.redo(); }, "new typing clears redo");
    check(contents(editor) == "helloworl!\n", "new edit stays in buffer");
    for (int i = 0; i < 10; ++i) editor.undo();
    check(contents(editor) == "\n", "undo all items");
    for (int i = 0; i < 10; ++i) editor.redo();
    check(contents(editor) == "helloworl!\n", "redo all items in order");
    editor.undo(); editor.redo(); editor.undo(); editor.redo();
    check(contents(editor) == "helloworl!\n", "alternating undo and redo");

    std::cout << "\nTEXT EDITOR: STRINGS\n";
    TextEditorBuffer<std::string> words;
    words.type("hello "); words.type("world"); words.type("!");
    std::cout << "Typed strings: "; words.displayBuffer();
    check(contents(words) == "hello world!\n", "string input");
    words.undo(); words.undo(); words.redo();
    std::cout << "Undo two, redo one: "; words.displayBuffer();
    check(contents(words) == "hello world\n", "whole-string undo and redo");
    words.redo();
    check(contents(words) == "hello world!\n", "restore all strings");
}

void setInput(char& item, const std::string& input) { item = input[0]; }
void setInput(std::string& item, const std::string& input) { item = input; }

template <typename T>
void menu(bool characterMode) {
    TextEditorBuffer<T> editor;
    std::string choice;
    while (true) {
        std::cout << "\nText Editor Buffer Simulation\n"
                  << "1. Type\n2. Undo\n3. Redo\n4. Display Buffer\n5. Exit\n"
                  << "Enter choice: ";
        if (!std::getline(std::cin, choice) || choice == "5") return;
        try {
            if (choice == "1") {
                std::string input;
                std::cout << (characterMode ? "Enter one character: " : "Enter a string: ");
                if (!std::getline(std::cin, input)) return;
                if (characterMode && input.size() != 1) {
                    std::cout << "Please enter exactly one character.\n";
                    continue;
                }
                T item;
                setInput(item, input);
                editor.type(item);
            } else if (choice == "2") editor.undo();
            else if (choice == "3") editor.redo();
            else if (choice != "4") {
                std::cout << "Please choose 1 through 5.\n";
                continue;
            }
            std::cout << "Buffer: "; editor.displayBuffer();
        } catch (const std::exception& error) {
            std::cout << error.what() << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--test") {
        std::string group = argc > 2 ? argv[2] : "all";
        if (group != "all" && group != "stacks" && group != "queues"
            && group != "lists" && group != "editor") {
            std::cerr << "Unknown test group\n";
            return 1;
        }
        try {
            if (group == "all" || group == "stacks") testStacks();
            if (group == "all" || group == "queues") testQueues();
            if (group == "all" || group == "lists") testLists();
            if (group == "all" || group == "editor") testEditor();
        } catch (const std::exception& error) {
            std::cout << "FAIL: unexpected exception: " << error.what() << '\n';
            ++failed;
        }
        std::cout << "\nResults: " << passed << " passed, " << failed << " failed\n";
        return failed == 0 ? 0 : 1;
    }
    std::string mode;
    std::cout << "Choose input mode (1 = characters, 2 = strings): ";
    if (!std::getline(std::cin, mode)) return 0;
    if (mode == "1") menu<char>(true);
    else if (mode == "2") menu<std::string>(false);
    else { std::cout << "Invalid mode.\n"; return 1; }
    return 0;
}

