#include <stdlib.h>
#include <util/delay.h>
#include "../Utilities/STD_TYPES.h"
#include <avr/eeprom.h>
#include "PhoneList.h"
#include "../MCAL/UART/UART_Interface.h"
#include "../Service/TopWayLCD_Interface.h"
#include "../Debug/debug_log.h"

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
	if (l == NULL) {
		DEBUG_LogError("Null list pointer in StoreListToEEPROM");
		return;
	}

	DEBUG_LogInfo("Starting to store list to EEPROM");
	DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "List size: ", l->size);
	
	current_eeprom_address = EEPROM_START_ADDRESS;
	
	// Store list size
	DEBUG_LogInfo("Writing list size to EEPROM");
	eeprom_write_byte((u8*)current_eeprom_address, l->size);
	current_eeprom_address++;

	Node* current = l->Head;
	u8 nodeCount = 0;
	
	while (current != NULL) {
		// Store phone number
		DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Writing node #", nodeCount);
		DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Writing to EEPROM address: ", current_eeprom_address);
		DEBUG_LogMessageWithValue(LOG_LEVEL_DEBUG, "Phone number: ", current->value);
		
		eeprom_write_block(current->value, 
						  (void*)current_eeprom_address, 
						  PHONE_NUMBER_LENGTH);
		current_eeprom_address += PHONE_NUMBER_LENGTH;
		
		// Store SMSCALL flag
		DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Writing SMSCALL flag: ", current->SMSCALL);
		eeprom_write_byte((u8*)current_eeprom_address, current->SMSCALL);
		current_eeprom_address++;
		
		current = current->Next;
		nodeCount++;
	}
	
	DEBUG_LogInfo("Finished storing list to EEPROM");
}

// Read the list from EEPROM
void ReadListFromEEPROM(List* l) 
{
	if (l == NULL) {
		DEBUG_LogError("Null list pointer in ReadListFromEEPROM");
		return;
	}

	DEBUG_LogInfo("Starting to read list from EEPROM");
	
	current_eeprom_address = EEPROM_START_ADDRESS;
	
	// Read list size
	u8 listSize = eeprom_read_byte((const u8*)current_eeprom_address);
	current_eeprom_address++;

	DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Stored list size: ", listSize);
	
	if (listSize == INVALID_EEPROM_VALUE) {
		DEBUG_LogWarning("Invalid EEPROM value detected, possibly first time use");
		return;
	}

	// Read each node
	u8 nodeCount = 0;
	while (listSize > 0) {
		u8 phoneNumber[PHONE_NUMBER_LENGTH + 1];
		
		DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Reading node #", nodeCount);
		DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Reading from EEPROM address: ", current_eeprom_address);
		
		// Read phone number
		eeprom_read_block(phoneNumber, 
						 (const void*)current_eeprom_address, 
						 PHONE_NUMBER_LENGTH);
		current_eeprom_address += PHONE_NUMBER_LENGTH;
		phoneNumber[PHONE_NUMBER_LENGTH] = '\0';
		
		DEBUG_LogMessageWithValue(LOG_LEVEL_DEBUG, "Read phone number: ", phoneNumber);
		
		// Read SMSCALL flag
		u8 smscall = eeprom_read_byte((const u8*)current_eeprom_address);
		DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Read SMSCALL flag: ", smscall);
		current_eeprom_address++;
		
		// Add to list
		AddNodeAtLast(l, phoneNumber, smscall);
		listSize--;
		nodeCount++;
	}
	
	DEBUG_LogValueInt(LOG_LEVEL_INFO, "Finished reading list from EEPROM, nodes found: ", nodeCount);
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
	if (PhoneNum == NULL) {
		DEBUG_LogError("Null phone number in AddNumToEEPROM");
		return;
	}
	
	DEBUG_LogMessageWithValue(LOG_LEVEL_INFO, "Adding phone number to EEPROM: ", PhoneNum);
	
	// Find the current list size stored in EEPROM
	current_eeprom_address = EEPROM_START_ADDRESS;
	u8 listSize = eeprom_read_byte((const u8*)current_eeprom_address);
	
	DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Current list size in EEPROM: ", listSize);
	
	// If first time or invalid value, initialize list size to 0
	if (listSize == INVALID_EEPROM_VALUE) {
		DEBUG_LogInfo("Initializing EEPROM with empty list");
		listSize = 0;
	}
	
	// Update list size
	listSize++;
	DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "New list size: ", listSize);
	eeprom_write_byte((u8*)current_eeprom_address, listSize);
	
	// Calculate where to add the new number (after all existing numbers)
	current_eeprom_address = EEPROM_START_ADDRESS + 1; // Skip past size byte
	current_eeprom_address += (listSize - 1) * (PHONE_NUMBER_LENGTH + 1); // Skip past existing entries
	
	DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Writing to EEPROM address: ", current_eeprom_address);
	
	// Store phone number
	eeprom_write_block(PhoneNum, (void*)current_eeprom_address, PHONE_NUMBER_LENGTH);
	current_eeprom_address += PHONE_NUMBER_LENGTH;
	
	// For now, store as Calling_list (1) by default
	DEBUG_LogInfo("Setting as Calling_list type (1)");
	eeprom_write_byte((u8*)current_eeprom_address, 1);
	
	DEBUG_LogInfo("Phone number added to EEPROM successfully");
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