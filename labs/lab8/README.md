# Lab 8 - Template Circular Sorted List

## Build and run

In this folder, using GCC (including GitHub Codespaces):

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o lab8
./lab8
```

Or from a Visual Studio Developer Command Prompt:

```bat
cl /std:c++17 /EHsc /W4 main.cpp /Fe:lab8.exe
lab8.exe
```

Run `lab8 int`, `lab8 double`, or `lab8 char` to display one type's tests.
The full run checks 82 conditions and exits with status 0 if all pass,
1 if a test fails, and 2 for an invalid command-line argument.

## Files to submit

- `sortedType.h`: template class and node declarations.
- `sortedType.cpp`: template member definitions, included by the header.
- `main.cpp`: tests for all three types and the required X Y Z example.
- `Lab8_Test_Runs.pdf`: screenshots of the actual captured test output.

Keep both SortedType files together. Compile `main.cpp`; it includes the
template definitions through `sortedType.h`. No `ItemType.h` is needed.
`test_output.txt` contains the complete run; `screenshots/` contains the
same captured-output images used in the PDF.

## Design and behavior

`listData` points to the tail (largest element), and `listData->next` points
to the head (smallest element), following the supplied `circle.cpp` model.
An empty list has a null tail; a singleton links to itself. `FindItem`
visits no more than one complete circle. The original `PutItem` name is
retained rather than the model's `InsertItem` name.

- Built-in `<` and `==` replace `ItemType::ComparedTo`.
- Duplicates are allowed; deletion removes the first matching occurrence.
- Searching for a missing item returns the unchanged argument and sets
  `found` to false. Deleting a missing item does nothing.
- `ResetList` restarts iteration at the head; `GetNextItem` wraps around.
  Call it `GetLength()` times to visit the list once. An empty call throws
  `std::out_of_range`.
- Successful insertion/deletion and `MakeEmpty` reset the traversal cursor.
- `PrintReverse` recursively visits exactly `length` nodes, then prints
  during recursion unwinding. It adds spaces between elements and a final
  newline, without changing links or the traversal cursor. An empty list
  prints a newline. It uses O(n) time and O(n) call-stack space, so extremely
  large lists can exceed the recursion stack.
- `MakeEmpty` breaks the circle before deleting nodes; the destructor calls it.
- Copying is explicitly disabled to prevent shallow pointer copies and
  double deletion; copying is not part of the assignment interface.
- `IsFull` probes a node allocation, following the textbook. A successful
  probe does not guarantee a later allocation will succeed.

Item types must be default/copy constructible, assignable, comparable with
`<` and `==`, and printable with `<<`. Comparisons must define a consistent
ordering (for example, use ordinary finite double values, not NaN).

## Verification

Built with Microsoft Visual C++ 19.44 using C++17, `/W4 /WX`, and the debug
runtime. All 82 checks pass: 27 each for int and double, and 28 for char.
Tests cover every public member, empty lists, one-node lists, duplicates,
all insertion/deletion positions, absent keys, wraparound, reuse after
clearing, and reverse output that preserves the traversal cursor.
