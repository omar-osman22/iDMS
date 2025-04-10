#include <stdlib.h>
#include "avr/delay.h"
#include "../Utilities/STD_TYPES.h"
#include <avr/eeprom.h>
#include "PhoneList.h"
#include "../MCAL/UART/UART_Interface.h"
#include "../Service/TopWayLCD_Interface.h"

#define PHONE_NUMBER_LENGTH 11
#define EEPROM_START_ADDRESS 0
#define INVALID_EEPROM_VALUE 0xFF

// Static variables
static u8 current_eeprom_address = 0;

// Static function prototypes
static u8 comparePhoneNumbers(const u8* num1, const u8* num2);
static Node* createNode(u8* data, u8 smscall);
static void freeNode(Node* node);

// Initialize the phone list
void PhoneList_INIT() 
{
	// Initialize any required hardware or variables
	current_eeprom_address = EEPROM_START_ADDRESS;
}

// Create an empty list
void CreateList(List* l) 
{
	if (l != NULL) {
		l->Head = NULL;
		l->size = 0;
	}
}

// Add a node at the end of the list
void AddNodeAtLast(List* pl, u8* data, u8 SMSCALL) 
{
	if (pl == NULL || data == NULL) return;

	Node* newNode = createNode(data, SMSCALL);
	if (newNode == NULL) return;

	if (pl->Head == NULL) {
		pl->Head = newNode;
	} else {
		Node* current = pl->Head;
		while (current->Next != NULL) {
			current = current->Next;
		}
		current->Next = newNode;
	}
	pl->size++;
}

// Print the list on the LCD screen
void PrintList(List* pl, u32 Calling_address, u32 SMS_address) 
{
	if (pl == NULL || pl->Head == NULL) return;

	Node* current = pl->Head;
	u8 call_index = 0;
	u8 sms_index = 0;

	while (current != NULL) {
		if (current->SMSCALL == 1) {
			LCD_SendString(Calling_address + (128 * call_index), current->value);
			call_index++;
		} else if (current->SMSCALL == 2) {
			LCD_SendString(SMS_address + (128 * sms_index), current->value);
			sms_index++;
		}
		current = current->Next;
	}
}

// Delete a node from the list
void Delete(u8* Data, List* pl) 
{
	if (pl == NULL || Data == NULL || pl->Head == NULL) return;

	Node* current = pl->Head;
	Node* previous = NULL;

	// Check if first node matches
	if (comparePhoneNumbers(current->value, Data)) {
		pl->Head = current->Next;
		freeNode(current);
		pl->size--;
		return;
	}

	// Check remaining nodes
	while (current != NULL) {
		if (comparePhoneNumbers(current->value, Data)) {
			previous->Next = current->Next;
			freeNode(current);
			pl->size--;
			return;
		}
		previous = current;
		current = current->Next;
	}
}

// Retrieve an element from the list
void RetrieveElement(u8* pe, const u8* Data, List* pl) 
{
	if (pl == NULL || Data == NULL || pe == NULL) return;

	Node* current = pl->Head;
	while (current != NULL) {
		if (comparePhoneNumbers(current->value, Data)) {
			for (u8 i = 0; i < PHONE_NUMBER_LENGTH; i++) {
				pe[i] = current->value[i];
			}
			pe[PHONE_NUMBER_LENGTH] = '\0';
			return;
		}
		current = current->Next;
	}
	
	// If number not found
	pe[0] = '0';
	pe[1] = '\0';
}

// Store the list in EEPROM
void StoreListToEEPROM(List* l) 
{
	if (l == NULL) return;

	current_eeprom_address = EEPROM_START_ADDRESS;
	
	// Store list size
	eeprom_write_byte((u8*)current_eeprom_address, l->size);
	current_eeprom_address++;

	Node* current = l->Head;
	while (current != NULL) {
		// Store phone number
		eeprom_write_block(current->value, 
						  (void*)current_eeprom_address, 
						  PHONE_NUMBER_LENGTH);
		current_eeprom_address += PHONE_NUMBER_LENGTH;
		
		// Store SMSCALL flag
		eeprom_write_byte((u8*)current_eeprom_address, current->SMSCALL);
		current_eeprom_address++;
		
		current = current->Next;
	}
}

// Read the list from EEPROM
void ReadListFromEEPROM(List* l) 
{
	if (l == NULL) return;

	current_eeprom_address = EEPROM_START_ADDRESS;
	
	// Read list size
	u8 listSize = eeprom_read_byte((const u8*)current_eeprom_address);
	current_eeprom_address++;

	if (listSize == INVALID_EEPROM_VALUE) return;

	// Read each node
	while (listSize > 0) {
		u8 phoneNumber[PHONE_NUMBER_LENGTH + 1];
		
		// Read phone number
		eeprom_read_block(phoneNumber, 
						 (const void*)current_eeprom_address, 
						 PHONE_NUMBER_LENGTH);
		current_eeprom_address += PHONE_NUMBER_LENGTH;
		phoneNumber[PHONE_NUMBER_LENGTH] = '\0';
		
		// Read SMSCALL flag
		u8 smscall = eeprom_read_byte((const u8*)current_eeprom_address);
		current_eeprom_address++;
		
		// Add to list
		AddNodeAtLast(l, phoneNumber, smscall);
		listSize--;
	}
}

// Add a phone number to the list
void PhoneList_ADD(u8* PhoneNum) 
{
	if (PhoneNum == NULL) return;
	Add_to_eeprom(PhoneNum);
}

// Delete a phone number from the list
void PhoneList_Delete(u8* PhoneNum) 
{
	// Implementation depends on your specific requirements
	// This could involve both list and EEPROM operations
}

// Search for a phone number in the list
void PhoneList_Search(u8 NumID) 
{
	// Implementation depends on your specific requirements
	// This could search by an ID instead of phone number
}

// Print the phone numbers
void print_numbers(void) 
{
	// Implementation depends on your display requirements
}

// Add a phone number to EEPROM
void Add_to_eeprom(u8* Num) 
{
	if (Num == NULL) return;
	AddNumToEEPROM(Num);
}

// Add a phone number to EEPROM
void AddNumToEEPROM(const u8* PhoneNum) 
{
	if (PhoneNum == NULL) return;
	
	// Find next available EEPROM address and store the number
	// Specific implementation depends on your EEPROM structure
}

// Static helper function to compare phone numbers
static u8 comparePhoneNumbers(const u8* num1, const u8* num2) 
{
	if (num1 == NULL || num2 == NULL) return 0;
	
	for (u8 i = 0; i < PHONE_NUMBER_LENGTH; i++) {
		if (num1[i] != num2[i]) return 0;
	}
	return 1;
}

// Static helper function to create a new node
static Node* createNode(u8* data, u8 smscall) 
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL) return NULL;

	for (u8 i = 0; i < PHONE_NUMBER_LENGTH; i++) {
		newNode->value[i] = data[i];
	}
	newNode->value[PHONE_NUMBER_LENGTH] = '\0';
	newNode->SMSCALL = smscall;
	newNode->Next = NULL;

	return newNode;
}

// Static helper function to free a node
static void freeNode(Node* node) 
{
	if (node != NULL) {
		free(node);
	}
}