# BTREE

This library implements a self balancing binary tree on a finite (or infinite)
resource. It supports normal tree operations: O-log-2 lookup times for
gets, puts and deletes. It works on static, dynamic and shared memory.
It works on files. It supports cursors, streaming and data integrity checks.
You can insert the same datum under multiple keys.

It requires a C compiler and make to build.

It requires perl to do the testing.
