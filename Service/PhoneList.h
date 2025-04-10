/*
 * PhoneList.h
 *
 *  Created on: Jun 8, 2023
 *      Author: Abdelrahman Hegazy
 */

#ifndef PHONELIST_H_
#define PHONELIST_H_

// Structure to represent a phone number node
typedef struct PhoneNum
{
	u8* Number;             // Phone number
	u8 Location;            // Location of the phone number
	struct PhoneNum* Next;  // Pointer to the next phone number node
} PhoneNum;

// Structure to represent a generic node
typedef struct Node
{
	u8 value[12];           // Value of the node
	u8 SMSCALL;             // SMSCALL flag
	struct Node* Next;      // Pointer to the next node
} Node;

// Structure to represent a list
typedef struct List
{
	Node* Head;             // Pointer to the head of the list
	u16 size;               // Size of the list
} List;

// Function to create an empty list
void CreateList(List* l);

// Function to add a node at the end of the list
void AddNodeAtLast(List* pl, u8* data, u8 SMSCALL);

// Function to print the list
void PrintList(List* pl, u32 Calling_address, u32 SMS_address);

// Function to delete a node from the list
void Delete(u8* Data, List* pl);

// Function to retrieve an element from the list
void RetrieveElement(u8* pe, const u8* Data, List* pl);

// Function to add a phone number to EEPROM
void AddNumToEEPROM(const u8* PhoneNum);

// Function to initialize the phone list
void PhoneList_INIT();

// Function to add a phone number to the list
void PhoneList_ADD(u8* PhoneNum);

// Function to delete a phone number from the list
void PhoneList_Delete(u8* PhoneNum);

// Function to search for a phone number in the list
void PhoneList_Search(u8 NumID);

// Function to print all the phone numbers in the list
void print_numbers(void);

// Function to add a phone number to EEPROM
void Add_to_eeprom(u8* Num);

// Function to store the list to EEPROM
void StoreListToEEPROM(List* l);

// Function to read the list from EEPROM
void ReadListFromEEPROM(List* l);

#endif /* PHONELIST_H_ */
