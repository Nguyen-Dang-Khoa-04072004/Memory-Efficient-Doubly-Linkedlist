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

![list](images/list.png)

# Why does it work?

![why does it work ?](images/work.png)

# APIs

## Iterators:
| getForwardIterator | Get the forward iterator |
|---| --- |
| getBackwardIterator | Get the backward iterator |

## Modifiers:

| push_back | Insert element at the end of the list |
|---| --- |
| push_front | Insert element at the beginning of the list |
| pop_back | Remove element from the end of the list |
| pop_front | Remove element from the beginning of the list |
| insert | Insert an element at the specified index |
| remove | Remove an element at the specified index |

## Element access:
| get | Get the element at the specified index|
|---| --- |

## Capacity:
| getSize | Get the size of list |
|---| --- |
| isEmpty | Check wether list is empty or not | 

# How to traverse the list ?

I use iterator pattern to traverse elements of a collection without exposing its underlying representation

```c++
#include "memory-efficient-doubly-linkedlist.cpp"

int main(){
    MemoryEfficientLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    MemoryEfficientLinkedList<int>::BaseIterator it = list.getBackwardIterator() : list.getForwardIterator();
    while(it.hasNext()){
        cout<<it.getValue()<<" ";
        it.next();
    }
    return 0;
}
```
# Play ground
```c++
#include "memory-efficient-doubly-linkedlist.cpp"


void display(MemoryEfficientLinkedList<int> &list, bool isReverse = false ){
    MemoryEfficientLinkedList<int>::BaseIterator it = isReverse ? list.getBackwardIterator() : list.getForwardIterator();
    while(it.hasNext()){
        cout<<it.getValue()<<" ";
        it.next();
    }
    cout<<endl;
}

int main(){
    MemoryEfficientLinkedList<int> list;

    // insert elements at the ending
    for(int i = 0; i <= 20; i++){
        list.push_back(i);
    }

    cout<<"Size: "<<list.getSize()<<endl;
    cout<<"Is is Empty? : "<<(list.isEmpty() ? "Yes" : "No")<<endl;
    // display list
    cout<<"Fordward: ";
    display(list);
    cout<<"Backward: ";
    display(list, true);

    // remove at the beginning
    cout<<"Remove at the beginning: ";
    list.pop_front();
    display(list); 

    // remove at the beginning
    cout<<"Remove at the ending: ";
    list.pop_back();
    display(list);

    // remove a element at position 5
    cout<<"Remove a element at position 5: ";
    list.remove(5);
    display(list); 

    // insert elements at the beginning
    for(int i = -1; i >= -20; i--){
        list.push_front(i);
    }
    cout<<"Insert elements at the beginning: ";
    display(list); 

    // get a element at 6 position
    cout<<"Element at position 6th is: "<<list.get(6)<<endl;
    return 0;
}
```
![result demo](images/demo.png)
