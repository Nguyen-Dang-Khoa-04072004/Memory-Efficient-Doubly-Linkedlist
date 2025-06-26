# A Memory-Efficient Doubly Linked List

This is an implementation of a doubly linked list using a memory-efficient approach.

# Why is it memory-efficient?

- In a conventional implementation, each node stores a pointer to the next item and a pointer to the previous item.
This means every node in a typical doubly linked list contains:
  - The data
  - A pointer to the next node
  - A pointer to the previous node
- In a memory-efficient implementation, we only store the XOR of the previous and next pointers, using the bitwise exclusive OR (⊕) operation.
With this approach, we can still traverse the list in both directions using just one pointer field per node.

# Why does it work?
