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