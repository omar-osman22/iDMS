#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// Node structure
typedef struct Node {
    int data;
    struct Node* next;
}Node;

// Linked list structure
typedef struct LinkedList {
    Node* head;
    int length;
}LinkedList;


// Function to create an empty linked list
LinkedList* createLinkedList();

// Function to check if the linked list is empty
int isEmpty(LinkedList* list);

// Function to get the length of the linked list
int length(LinkedList* list);

// Function to insert an element at the beginning of the linked list
void insertAtBeginning(LinkedList* list, int data);

// Function to insert an element at the end of the linked list
void insertAtEnd(LinkedList* list, int data);

// Function to insert an element at a specific position in the linked list
void insertAtPosition(LinkedList* list, int data, int position);

// Function to delete the first occurrence of an element from the linked list
void deleteElement(LinkedList* list, int data);

// Function to display the linked list
void display(LinkedList* list);

// Function to free the memory occupied by the linked list
void destroyLinkedList(LinkedList* list);

#endif
