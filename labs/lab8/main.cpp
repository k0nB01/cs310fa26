#include "sortedType.h"
#include <algorithm>
#include <initializer_list>
#include <sstream>
#include <string>
#include <vector>

int checks = 0;
int failures = 0;

void Check(bool condition, const std::string& label)
{
    ++checks;
    if (!condition)
        ++failures;
    std::cout << (condition ? "[PASS] " : "[FAIL] ") << label << '\n';
}

template <class T>
std::vector<T> ReadList(SortedType<T>& list)
{
    std::vector<T> values;
    list.ResetList();
    for (int i = 0; i < list.GetLength(); ++i)
        values.push_back(list.GetNextItem());
    return values;
}

template <class T>
std::string ReverseText(const SortedType<T>& list)
{
    std::ostringstream output;
    std::streambuf* original = std::cout.rdbuf(output.rdbuf());
    try
    {
        list.PrintReverse();
    }
    catch (...)
    {
        std::cout.rdbuf(original);
        throw;
    }
    std::cout.rdbuf(original);
    return output.str();
}

template <class T>
bool Contains(SortedType<T>& list, T value)
{
    bool found = false;
    const T original = value;
    const T result = list.GetItem(value, found);
    return found && result == original && value == original;
}

template <class T>
void RunTests(const std::string& name, T low, T middle, T high,
              T below, T between, T above)
{
    std::cout << "\n=== SortedType<" << name << "> ===\n";
    SortedType<T> list;
    Check(list.GetLength() == 0, "Constructor: empty list");
    Check(!list.IsFull(), "IsFull: memory available");
    bool found = true;
    T query = middle;
    Check(list.GetItem(query, found) == middle && !found,
          "GetItem: empty list");
    Check(ReverseText(list) == "\n", "PrintReverse: empty list");
    bool threw = false;
    try { list.GetNextItem(); }
    catch (const std::out_of_range&) { threw = true; }
    Check(threw, "GetNextItem: empty-list exception");
    list.DeleteItem(middle);
    list.MakeEmpty();
    Check(list.GetLength() == 0, "DeleteItem / MakeEmpty: empty list");

    list.PutItem(middle);
    list.PutItem(high);
    list.PutItem(low);
    Check(ReadList(list) == std::vector<T>{low, middle, high},
          "PutItem: empty, tail, head; sorted traversal");
    std::cout << "Forward: ";
    for (T value : ReadList(list)) std::cout << value << ' ';
    std::cout << "\nReverse: ";
    list.PrintReverse();
    std::ostringstream expectedReverse;
    expectedReverse << high << ' ' << middle << ' ' << low << '\n';
    Check(ReverseText(list) == expectedReverse.str(), "PrintReverse: exact reversed order");
    Check(list.GetNextItem() == low, "GetNextItem: tail wraps to head");
    list.ResetList();
    Check(list.GetNextItem() == low, "ResetList: restart at head");
    list.PrintReverse();
    Check(list.GetNextItem() == middle, "PrintReverse preserves traversal position");
    Check(Contains(list, low) && Contains(list, middle) && Contains(list, high),
          "GetItem: find head, middle, tail");
    bool absent = true;
    for (T value : {below, between, above})
    {
        query = value;
        found = true;
        T result = list.GetItem(query, found);
        absent = absent && !found && query == value && result == value;
    }
    Check(absent, "GetItem: missing below, between, above");

    list.PutItem(between);
    Check(ReadList(list) == std::vector<T>{low, between, middle, high},
          "PutItem: insert between existing nodes");
    list.DeleteItem(between);
    list.PutItem(middle);
    Check(ReadList(list) == std::vector<T>{low, middle, middle, high},
          "PutItem: duplicate; length is four");
    list.DeleteItem(middle);
    Check(ReadList(list) == std::vector<T>{low, middle, high},
          "DeleteItem: remove just one duplicate");
    for (T value : {below, between, above}) list.DeleteItem(value);
    Check(ReadList(list) == std::vector<T>{low, middle, high},
          "DeleteItem: missing values leave list unchanged");
    list.ResetList();
    list.GetNextItem(); // currentPos now points to the soon-to-be-deleted middle.
    list.DeleteItem(middle);
    Check(list.GetNextItem() == low &&
              ReadList(list) == std::vector<T>{low, high},
          "DeleteItem: middle; traversal safely resets");
    list.DeleteItem(low);
    Check(ReadList(list) == std::vector<T>{high}, "DeleteItem: head");
    list.PutItem(low);
    list.DeleteItem(high);
    Check(ReadList(list) == std::vector<T>{low}, "DeleteItem: tail");
    std::ostringstream one;
    one << low << '\n';
    Check(ReverseText(list) == one.str(), "PrintReverse: singleton");
    list.DeleteItem(low);
    Check(list.GetLength() == 0, "DeleteItem: only node");
    list.PutItem(high);
    list.PutItem(high);
    list.PutItem(high);
    Check(ReadList(list) == std::vector<T>{high, high, high},
          "PutItem: all-equal circular list");
    list.DeleteItem(high);
    Check(ReadList(list) == std::vector<T>{high, high},
          "DeleteItem: all-equal list stays circular");
    list.MakeEmpty();
    list.MakeEmpty();
    Check(list.GetLength() == 0 && ReverseText(list) == "\n",
          "MakeEmpty: populated list and repeated call");
    list.PutItem(middle);
    Check(list.GetNextItem() == middle && list.GetLength() == 1,
          "PutItem: reuse after MakeEmpty");
    { SortedType<T> temporary; temporary.PutItem(high); temporary.PutItem(low); }
    Check(true, "Destructor: populated list returns normally");
}

int main(int argc, char* argv[])
{
    const std::string mode = argc > 1 ? argv[1] : "all";
    if (argc > 2 || (mode != "all" && mode != "int" &&
                     mode != "double" && mode != "char"))
    {
        std::cerr << "Usage: lab8 [all|int|double|char]\n";
        return 2;
    }
    std::cout << "LAB 8 - TEMPLATE CIRCULAR SORTED LIST\n";
    try
    {
        if (mode == "all" || mode == "int")
            RunTests("int", 10, 20, 30, 5, 15, 40);
        if (mode == "all" || mode == "double")
            RunTests("double", 1.5, 2.5, 3.5, 0.5, 2.0, 4.5);
        if (mode == "all" || mode == "char")
        {
            RunTests("char", 'A', 'M', 'Z', '0', 'H', 'z');
            SortedType<char> example;
            for (char value : {'Y', 'Z', 'X'}) example.PutItem(value);
            std::cout << "Assignment example X Y Z -> ";
            example.PrintReverse();
            Check(ReverseText(example) == "Z Y X\n", "Required X Y Z example");
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Unexpected exception: " << error.what() << '\n';
        return 1;
    }
    std::cout << "\nRESULT: " << checks - failures << '/' << checks
              << " checks passed; " << failures << " failed.\n";
    return failures == 0 ? 0 : 1;
}
