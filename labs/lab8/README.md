# Lab 8 - Template Circular Sorted List

Build and run in this folder:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o lab8
./lab8
```

Submit sortedType.h, sortedType.cpp, main.cpp, and Lab8_Test_Runs.pdf.
The header includes the template definitions from sortedType.cpp.

The test program uses only iostream, ordinary variables, for loops,
and two short template functions. It prints actual and expected results
for int, double, and char. No algorithm, initializer_list, sstream, string,
or vector headers are used. Run the program once to see all three types.

The list uses a tail pointer; tail->next points to the first node.
Duplicates are allowed. DeleteItem removes one matching item and does
nothing if it is missing. GetNextItem wraps around the circle, and
ResetList starts again at the first item. Only call GetNextItem on a
nonempty list; an empty call throws std::out_of_range.
PrintReverse prints during recursive unwinding without changing the list.
MakeEmpty breaks the circle and frees the nodes. The destructor calls it.
Copying is disabled to prevent accidental sharing of owned nodes.

The list itself retains iostream for printing, new for the textbook's
IsFull allocation check, and stdexcept for an empty-list error.
The PDF and screenshots show the simplified program's actual output.
Expected values are printed alongside the results for easy comparison;
these are demonstrations, not the previous automated 82-check test suite.
