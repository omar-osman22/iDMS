#include <stdio.h>
#include <stdlib.h>

#include "LinkedList.h"

// Function to create an empty linked list
LinkedList* createLinkedList() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->length = 0;
    return list;
}

// Function to check if the linked list is empty
int isEmpty(LinkedList* list) {
    return list->head == NULL;
}

// Function to get the length of the linked list
int length(LinkedList* list) {
    return list->length;
}

// Function to insert an element at the beginning of the linked list
void insertAtBeginning(LinkedList* list, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = list->head;
    list->head = newNode;
    list->length++;
}

// Function to insert an element at the end of the linked list
void insertAtEnd(LinkedList* list, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (isEmpty(list)) {
        list->head = newNode;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    list->length++;
}

// Function to insert an element at a specific position in the linked list
void insertAtPosition(LinkedList* list, int data, int position) {
    if (position < 0 || position > list->length) {
        printf("Invalid position\n");
        return;
    }

    if (position == 0) {
        insertAtBeginning(list, data);
    } else if (position == list->length) {
        insertAtEnd(list, data);
    } else {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = data;

        Node* current = list->head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;

        list->length++;
    }
}

// Function to delete the first occurrence of an element from the linked list
void deleteElement(LinkedList* list, int data) {
    if (isEmpty(list)) {
        printf("Linked list is empty\n");
        return;
    }

    Node* current = list->head;
    Node* previous = NULL;

    while (current != NULL && current->data != data) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Element not found\n");
        return;
    }

    if (previous == NULL) {
        list->head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
    list->length--;
}

// Function to display the linked list
void display(LinkedList* list) {
    if (isEmpty(list)) {
        printf("Linked list is empty\n");
        return;
    }

    Node* current = list->head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Function to free the memory occupied by the linked list
void destroyLinkedList(LinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}


