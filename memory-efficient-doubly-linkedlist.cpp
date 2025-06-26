#include <iostream>
#include <cstdint>

using namespace std;

template <class T>
class BaseIterator;
template <class T>
class ForwardIterator;
template <class T>
class BackwardIterator;


template <class T>
class Node {
    public:
        T value;
        Node * ptrdiff;
        Node(T value){
            this->value = value;
            this->ptrdiff = nullptr;
        }
};

template <class T>
class MemoryEfficientLinkedList{
    private:
        int size;
        Node<T> * head;
        Node<T> * tail;
        Node<T>* XOR(Node<T> * a, Node<T> * b) {
            return reinterpret_cast<Node<T> *>(
                reinterpret_cast<uintptr_t>(a) ^ reinterpret_cast<uintptr_t>(b)
            );
        }
    public:
        class BaseIterator {
            protected:
                MemoryEfficientLinkedList<T> * list;
                Node<T>  * previousNode;
                Node<T>  * currentNode;
                Node<T>* XOR(Node<T> * a, Node<T> * b) {
                    return reinterpret_cast<Node<T> *>(
                        reinterpret_cast<uintptr_t>(a) ^ reinterpret_cast<uintptr_t>(b)
                    );
                }
            public:
                BaseIterator(MemoryEfficientLinkedList<T> * list){
                    this->list = list;
                    previousNode = nullptr;
                } 
                bool hasNext(){
                    return currentNode != nullptr;
                }
                T getValue(){
                    return currentNode->value;
                }
                BaseIterator * next(){
                    Node<T> * nextNode = XOR(currentNode->ptrdiff, previousNode); 
                    previousNode = currentNode;
                    currentNode = nextNode;
                    return this;
                }
        };



        class ForwardIterator : public BaseIterator{
            public:
                ForwardIterator(MemoryEfficientLinkedList<T> * list) : BaseIterator(list){
                    this->currentNode = this->list->head;
                }
        };


        class BackwardIterator : public BaseIterator{
            public:
                BackwardIterator(MemoryEfficientLinkedList<T> * list) : BaseIterator(list){
                    this->currentNode = this->list->tail;
                }
        };

        MemoryEfficientLinkedList(){
            this->size = 0;
            this->head = nullptr;
            this->tail = nullptr;
        }
        
        ~MemoryEfficientLinkedList(){
            if(head == nullptr){
                return;
            } 
            Node<T> * previousNode = nullptr;
            Node<T> * currentNode = head;
            while (currentNode != nullptr)
            {
                delete previousNode;
                Node<T> * nextNode = XOR(currentNode->ptrdiff, previousNode); 
                previousNode = currentNode;
                currentNode = nextNode;
            }
            delete currentNode;
        }
        
        BaseIterator  getBackwardIterator(){
            return BackwardIterator(this);
        }


        BaseIterator  getForwardIterator(){
            return ForwardIterator(this);
        }


        int getSize(){
            return this->size;
        }

        
        bool isEmpty(){
            return this->size == 0;
        }

        /*
            Function: Get the element at the specified index
            Parameters: 
                - index: position of the element
            Returns: value of the element at that position
        */
    
        T get(int index){
            Node<T> * previousNode = nullptr;
            if(index < 0 || index >= this->size ){
                throw runtime_error("Index is out of range");
            }
            bool isSearchFromHead = index < this->size - 1 - index;
            Node<T> * currentNode =  isSearchFromHead ? this->head : this->tail;
            int count = isSearchFromHead ? 0 : this->size - 1; 
            while (currentNode != nullptr)
            {
                if(count == index){
                    break;
                }
                Node<T> * nextNode = XOR(currentNode->ptrdiff, previousNode); 
                previousNode = currentNode;
                currentNode = nextNode;
                count += isSearchFromHead ? 1 : -1;
            }
            return currentNode->value;
        }

         /*
            Function: Insert an element at the specified index
            Parameters:
                - index: position to insert at
                - value: value of the new element
        */
        
        void insert(int index, T value){
            Node<T> * newNode = new Node<T>(value);
            if(this->head == nullptr){
                this->head = this->tail = newNode;
            }else{
                /*
                    input : A -> B (ptrdiff = A XOR D)-> D (ptrdiff = B XOR E) -> E
                    expect: A -> B (ptrdiff = A XOR C) -> C (ptrdiff = B XOR D) -> D (ptrdiff C XOR E)-> E
                */
                Node<T> * previousNode = nullptr;
                Node<T> * currentNode =  this->head;
                if(index <= 0){
                    push_front(value);
                }else if(index >= this->size){
                    push_back(value);
                }else{
                    for(int i = 0 ; i < index - 1; i++){
                        Node<T> * nextNode = XOR(currentNode->ptrdiff, previousNode); 
                        previousNode = currentNode;
                        currentNode = nextNode;
                    }
                    Node<T> * nodeNext = XOR(currentNode->ptrdiff, previousNode);
                    nodeNext->ptrdiff = XOR(XOR(nodeNext->ptrdiff,currentNode),newNode);
                    currentNode->ptrdiff = XOR(previousNode,newNode);
                    newNode->ptrdiff = XOR(currentNode,nodeNext);
                }
            }
            this->size++;
        }

        /*
            Function: Remove an element at the specified index
            Parameters:
                - index: position to insert at
        */
        void remove(int index){
            if(index < 0 || index >= this->size ){
                throw runtime_error("Index is out of range");
            }
            if(this->head == nullptr){
                return;
            }else{
                /*
                    input : A -> B (ptrdiff = A XOR C)-> C (ptrdiff = B XOR D) -> D (ptrdiff = C XOR E) -> E
                    expect: A -> B (ptrdiff = A XOR D) -> D (ptrdiff = B XOR E) -> E
                */
                Node<T> * previousNode = nullptr;
                Node<T> * currentNode =  this->head;
                for(int i = 0 ; i < index ; i++){
                    Node<T> * nextNode = XOR(currentNode->ptrdiff, previousNode); 
                    previousNode = currentNode;
                    currentNode = nextNode;
                }
                Node<T> * nodeNext = XOR(currentNode->ptrdiff, previousNode);
                previousNode->ptrdiff = XOR(XOR(previousNode->ptrdiff, currentNode), XOR(currentNode->ptrdiff,previousNode));
                nodeNext->ptrdiff = XOR(XOR(nodeNext->ptrdiff,currentNode),previousNode);
                delete currentNode;
            }
            this->size--;
        } 

        /*
        Function: Insert element at the beginning of the list
        Parameters:
            - value: new element's value
        */
        
        void push_front(T value){
            Node<T> * newNode = new Node<T>(value);
            /*
                Check if head is nullptr then just set head, tail equal to new node
            */
            if(this->head == nullptr){
                this->head = this->tail = newNode;
            }else{
                /*
                    head (ptrdiff = next node)-> ... -> nullptr
                    new node -> head  (new node XOR next node)-> ... -> nullptr
                */
    
                // Update new node's ptrdiff and head's ptrdiff
                this->head->ptrdiff = XOR(newNode, head->ptrdiff);
                newNode->ptrdiff = head;
                this->head = newNode;
            } 
            this->size++;
        }

        /*
            Function: Remove element from the beginning of the list
        */ 
        
        void pop_front(){
             /*
                input: A (ptrdiff = B)-> B (ptrdiff = A XOR C) -> C
                expect: B (ptrdif = C)-> C
            */

            // Update next node's ptrdiff
            Node<T> * nextNode = head->ptrdiff;
            nextNode->ptrdiff = XOR(nextNode->ptrdiff, head);
            delete head;
            this->head = nextNode;
            this->size--;
        }

        /*
            Function: Insert element at the end of the list
            Parameters:
                - value: new element's value
        */
        
        void push_back(T value){
            Node<T> * newNode = new Node<T>(value);
            /*
                Check if head is nullptr then just set head, tail equal to new node
            */
            if(this->head == nullptr){
                this->head = this->tail = newNode;
            }else{
                /*
                    input: A -> B (ptrdiff = A)-> nullptr
                    expect: A -> B (ptrdiff = A XOR C )-> C (ptrdiff = B)-> nullptr
                */
                // Update ptrdiff of old tail
                this->tail->ptrdiff = XOR(tail->ptrdiff, newNode);
    
                // Update ptrdiff of current tail
                newNode->ptrdiff = tail;
                this->tail = newNode;
            }
            this->size++;
        }


        /*
            Function: Remove element from the end of the list
        */
        
        void pop_back(){
            /*
                input: A -> B (ptrdiff = A XOR C) -> C (ptrdiff = B) -> nullptr
                expect: A -> B (ptrdiff = A) -> nullptr
            */   
            Node<T> * previousNode = tail->ptrdiff; // bacause tail's ptrdiff = prev XOR nullptr
            
            // Update prev's ptrdiff
            previousNode->ptrdiff = XOR(previousNode->ptrdiff, tail); 
            // Deallocate tail's memory and update tail
            delete tail;
            this->tail = previousNode;
            this->size--;
        }
};


