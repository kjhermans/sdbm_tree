# BTREE

This library implements a self balancing binary tree on a finite (or infinite)
resource. It supports normal tree operations: O(log2 n) lookup times for
gets, puts and deletes. It works on static, dynamic and shared memory.
It works on files. It supports streaming and data integrity checks.
You can insert the same datum under multiple keys, or use the same key
multiple times (or enforce that such a situation expressly doesn't occur).
It supports cursors, with partial matching moves and/or exact moves.
You can provide your own comparison function callback.

It requires a C compiler and make to build.

It requires perl to do the testing.

What it doesn't do: it has no transaction model other than locking.

Comparison to Sleepycat: I have no idea.
