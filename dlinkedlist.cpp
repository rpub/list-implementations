/*
 * Title:		Doubly Linked List, Methods
 * Purpose:		Method implementations
 * Author:		Carlos Arias/Rami Isaac
 * Date:		May 14, 2020
 */
#include "object.h"
#include "dlinkedlist.h"

#include <string>
#include <iostream>
#include <sstream>

// using namespace std;
using std::string;
using std::istream;
using std::ostream;
using std::stringstream;


/**
 * Default constructor
 * Just sets the head and tail to nullptr
 */
DoubleLinkedList::DoubleLinkedList() : _head(nullptr), _tail(nullptr){}

/**
 * Copy Constructor
 * Creates a deep copy of the list. It creates a clone each of the
 * elements in the original list and inserts them into the copy,
 * this insertion is done using the Insert method.
 * @param DoubleLinkedList &other
 */
DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList &other) : _head(nullptr){
    for (Node* tmp = other._head; tmp != nullptr; tmp = tmp->next){
        Insert(tmp->data->Clone(), _size);
    }
}

/**
 * Copy assignment operator
 * Enables the deep copy assignment using the operator = overload. This
 * method is kept private to avoid usage from clients of the class.
 * This method also relies on the Clone method to create copies of the
 * elements.
 * @param rhs the object to be copied into this
 * @return this to enable cascade assignments
 */
DoubleLinkedList &DoubleLinkedList::operator=(const DoubleLinkedList &rhs) {
    for (Node* tmp = rhs._head; tmp != nullptr; tmp = tmp->next){
        Insert(tmp->data->Clone(), _size);
    } // Sets elements equal to rhs
	return *this;
}

/**
 * Destructor
 * Frees the dynamic memory allocated for the list, it does so by
 * traversing the list and releasing the memory of each element and
 * then its node.
 */
DoubleLinkedList::~DoubleLinkedList() {
    Node* tempNode = _head;
    while (_head != nullptr){
        _head = tempNode->next;
        if (tempNode->data)
            delete tempNode->data;
        delete tempNode;
        tempNode = _head;
    }
}

/**
 * Insert()
 * Inserts an element into a given position.
 * See comments in method below for detailed explanation.
 * @param element what the client wants to insert into the list
 * @param position the position where the element is to be inserted
 * @return true if it was possible to insert, false otherwise. It will not be able
 * to insert if the position is invalid.
 */
bool DoubleLinkedList::Insert(Object *element, size_t position) {

    // If position is valid
    if (position > _size){
        return false;
    }

    Node* neo = new Node();  // Allocate memory for new node, 'neo'
    neo->data = element;     // Assign data to neo
    neo->next = nullptr;     // Next ptr, null by default
    neo->previous = nullptr; // Previous ptr, null by default

    if (_size == 0){
        /* --- INSERT NODE AT FRONT
         * If the list's head pointer is null (list is empty), the algorithm...
         * 1. Points the list's head and tail pointers to the new node.
         */
        /* [ 1a ] */ _head = neo;
        /* [ 1b ] */ _tail = neo;
    } else if (position == 0) {
        /* --- INSERT NEW, UPDATED LIST _HEAD
         * If the list is not empty and user wishes to insert neo as head, the algoirthm...
         * 1. Links the list
         * 2. Assigns the 'new' neo head
         */
        neo->next = _head;     // Assigns next of neo to _head
        _head->previous = neo; // Assigns previous of head to neo;
        _head = neo;             // Assigns _head to neo
    } else if (position == _size - 1) {
        /* --- INSERT NODE AFTER TAIL
         *  the list's head pointer is not null (list is not empty) and curNode
         *  points to the list's tail node, the new node is inserted after the tail node.
         *
         *  The algorithm...
         *
         * 1. Points the tail node's next pointer to the new node
         * 2. Points the new node's previous pointer to the list's tail node
         * 3. Points the list's tail pointer to the new node
         * 4. Points the new node's next pointer to null (tail node)
         */
        /* [ 1 ] */ _tail -> next = neo;
        /* [ 2 ] */ neo -> previous = _tail;
        /* [ 3 ] */ _tail = neo;
        /* [ 4 ] */ neo -> next = nullptr;
    } else {
        /* --- INSERT NODE IN MIDDLE
         * If the list's head pointer is not null (list is not empty) and curNode does
         * not point to the list's tail node, the algorithm updates the current, new, and
         * successor nodes' next and previous pointers to achieve the ordering
         * {curNode newNode sucNode}, which requires four pointer updates:
         *
         * 1. Point the new node's next pointer to sucNode
         * 2. Point the new node's previous pointer to curNode
         * 3. Point curNode's next pointer to the new node
         * 4. Point sucNode's previous pointer to the new node.
         */
        Node* tmp = _head; // Allocate memory for a temporary node

        // Traverse through list
        for (size_t i = 0; i < position - 1; i++){
            tmp = tmp -> next;
        }

        /* [ 1 ] */ neo -> next = tmp -> next; // Assign next of neo equal to next of previous node
        /* [ 2 ] */ neo -> previous = tmp;     // Assign previous of neo to previous node
        /* [ 3 ] */ tmp -> next = neo;         // Assign next of previous node to neo
        if(neo -> next != nullptr) // Check if neo is the tail
            /* [ 4 ] */ neo -> next -> previous = neo; // Assign previous of node next to neo, to neo
    }
    _size++;
    return true;
}

/**
 * Index Of
 * Searches for the position of an element in the list.
 * The method performs a linear search, traversing the list, for the given
 * element.
 * If the element is found it returns the position, if it is not found it
 * returns -1. It is important to note that for this operation to succeed it
 * is necessary to override the method Equals of all the types inserted into
 * the list.
 * @param element the object that the client is searching for.
 * @return the position of the element if found, -1 otherwise.
 */
int DoubleLinkedList::IndexOf(const Object *element) const {
    size_t index = 0;
    for (Node* tmp = _head; tmp != nullptr; tmp = tmp->next){
        if (tmp->data->Equals(*element)){
            return index;
        }
        index++;
    }
    return -1;
}

/**
 * Remove()
 * Removes the element at position, when the position is valid. It returns
 * the pointer to the removed element. This method does not release any
 * dynamically allocated object. It is left to the client to release the
 * memory of the removed element. After removing the element, the list needs
 * to be correctly link to avoid losing the reference to the elements after
 * the removed element.
 * @param position the position of the element to be removed.
 * @return the pointer to the object in that position if the position was valid,
 * nullptr otherwise
 */
Object *DoubleLinkedList::Remove(size_t position) {

    // If position is valid
    if (position >= _size) {
        return nullptr;
    }

    Node* tmp = nullptr;
    Object* retVal = nullptr;

    if(position == 0){
        retVal = _head->data;
        _head->data = nullptr;
        tmp = _head->next;
        _head->next = nullptr;
        if(_tail == _head){
            _tail = tmp;
        }
        delete _head;
        _head = tmp;
    } else {

        tmp = _head; // Traverse list
        for (size_t i = 0; i < position -1; i++){
            tmp = tmp->next;
        }

        Node* toRemove = tmp->next; // New node is going to be after current node
        retVal = toRemove->data;    // Holds value of new node
        tmp->next = toRemove->next;

        // Determines if tail / middle node
        if(toRemove->next != nullptr){
            toRemove->next->previous = tmp;
            toRemove->next = nullptr; // Tails
        } else { //element removed is at end of list
            _tail = tmp;
        }
        toRemove->previous = nullptr; // Clears original node
        toRemove->data = nullptr;     // Clears original node
        delete toRemove;              // Deletes original node
    }
    _size--;
    return retVal;
}

/**
 * Get Position
 * Returns the element at a given position. Note that it is not a copy of the
 * element.
 * @param position the position of the element to retrieve.
 * @return a pointer to the element if the position is valid, nullptr otherwise
 */
Object *DoubleLinkedList::Get(size_t position) const {
    if (position >= _size){
        return nullptr;
    }
    Node* tmp = _head;
    for (size_t i = 0; i < position; i++) {
        tmp = tmp->next;
    }
    return tmp->data;
}

/**
 * ToString()
 * Creates a string representation of the list. This representation
 * will be the string representation of each of the elements, surrounding
 * the whole list with curly braces and separating the elements by comma.
 * Example: {2, 6, 8}
 * @return a string representation of the list
 */
string DoubleLinkedList::ToString() const {
    stringstream retVal;
    retVal << "{";
    Node* tmp;
    for (tmp = _head; tmp->next != nullptr; tmp = tmp->next) {
        retVal << tmp->data->ToString() << ", ";
    }
    retVal << tmp->data->ToString() << "}";
    return retVal.str();
}

/**
 * Clear()
 * Very similar to the destructor, this method releases all the elements
 * of the list. It also sets the size to zero, and the head to nullptr.
 * This method makes the list become empty.
 */
void DoubleLinkedList::Clear() {
    Node* tempNode = _head;
    while (_head != nullptr){
        _head = tempNode->next;
        if (tempNode->data)
            delete tempNode->data;
        delete tempNode;
        tempNode = _head;
    }
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
}
