# Lab 6.1 - Text Editor Buffer

This lab uses hand-written template classes for an array stack, a circular
linked stack, a circular array queue, a circular linked queue, a singly
linked list, and a circular linked list. The editor uses the circular
linked stack and circular linked queue. Optional operation history is omitted.

## Build and run

From this folder in the Codespace:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o lab61
./lab61
```

Choose character or string mode, then use the menu to type, undo, redo,
or display the buffer. Spaces are kept. Each string is one edit, so undo
removes the whole string. An empty string is also a valid string edit.

## Tests

```sh
./lab61 --test
./lab61 --test stacks
./lab61 --test queues
./lab61 --test lists
./lab61 --test editor
```

The full run has 52 checks. A failing check returns a nonzero exit code.
See `test_output.txt` and the report for results and screenshots.

## Design notes

- Only standard C++ headers are used: iostream, string, stdexcept,
  limits, and sstream. The last one captures display output for tests.
- The array stack starts at 100 and doubles when full. `isFull()` means
  the currently allocated array is full, not that the next push must fail.
  Capacity arithmetic overflow throws `overflow_error`; allocation failure
  uses the standard allocation exception.
- The array queue has a capacity of 100 and throws on overflow.
- The redo queue keeps FIFO operations. After undo, older queue entries
  are rotated behind the newest undone item. This satisfies both the
  required queue and the instruction to redo the last undone item.
- Typing after undo clears the redo queue.
- The ten-character test uses `helloworld`. `hello world` has 11 characters.
- Copying the owning data structures is disabled to prevent shallow
  pointer copies and double deletion. Copying is not required by the lab.

`Lab6.1_Report.pdf` includes all source code, test screenshots, discussion,
and reflection. `report.md` keeps the discussion and reflection editable.

