# Lab 6.1 - Text Editor Buffer

## Overview

This program uses templates to build the stacks, queues, and linked lists.
The text editor can take characters or strings. It can type, undo, redo,
and display the buffer. I left out the optional history to keep it simple.

## Discussion

Templates let me use the same class with different data types. Instead of
writing separate stacks for characters and strings, I can change the type
inside the angle brackets. The operations stay the same. The tests use
integers for the basic structures and characters and strings for the editor.

The main thing to watch with the circular structures is the last node's
next pointer. It has to point back to the first node. An empty structure
and a structure with one node need their own cases. When the last node is
deleted, the pointer needs to become null. Otherwise, the program could
keep using a node that was already deleted. Display also needs a stopping
point because a circular list never reaches a null pointer.

Arrays are straightforward and store their elements next to each other.
The array stack can have unused spaces, and resizing takes O(n) time
because the items have to be copied. A normal push is O(1), and push is
O(1) amortized over many pushes. Pop and top are O(1). The array queue uses
wraparound, so enqueue, dequeue, and front are all O(1). Its capacity is 100.

The linked stack and queue grow one node at a time, so they do not need
to copy every item when they grow. Their main operations are O(1), but
each node uses extra space for a pointer. Both linked lists use O(1)
front insertion, rear insertion, and front deletion. Rear deletion is
O(n) because the code has to find the node before the last one. Display
is O(n). The linked structures use O(n) space. The array stack uses
O(capacity) space, while the fixed array queue always has 100 slots.

The redo order was an important detail. A regular queue returns the first
item added, but redo needs the most recently undone item. After undo, the
program moves the older queue entries behind the new one. This keeps the
queue's normal behavior while giving the editor the right redo order.
Undo is O(r), where r is the number of items already waiting for redo.
Redo is O(1). Typing is O(r) when it clears old redo items. Displaying the
buffer is O(n) time and uses an extra O(n) stack to show the text in order.
These costs count stored items; copying a string also depends on its length.

## Reflection

This lab helped me understand how stacks and queues work together in a
program. The individual operations were easier to follow once I could
see them change the text in the buffer. I also learned that getting the
order right matters just as much as storing the right values.

Part 1 took 1 hour and 30 minutes. With the provided examples, it was
kind of simple to put everything together. Part 2 took 1 hour because it
was a short segment of this lab. Part 3 took 2 hours and 35 minutes
because some outputs looked off or were flat out wrong. The total was
5 hours and 5 minutes.

Testing took the longest. Looking at the output made it easier to notice
when the items were coming back in the wrong order. One thing that stood
out was that undoing and redoing one item is not enough to check the
editor. Doing it several times makes order problems much easier to see.
The empty cases and the one-node cases also needed attention.

## Test results

The automated run passed all 52 checks. The checks cover both stacks,
both queues, both linked lists, and the character and string editors.
They include empty operations, stack growth, queue overflow and
wraparound, deleting the last node, multiple undo/redo operations, and
clearing redo after new typing. The PDF includes screenshots with captions
and the full source code from structures.h and main.cpp.

