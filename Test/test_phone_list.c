/*
 * test_phone_list.c
 *
 * Terminal-based testing utility for PhoneList functionality
 */

#include <stdio.h>
#include <string.h>
#include "../Utilities/STD_TYPES.h"
#include "../Utilities/BIT_MATH.h"
#include "../Service/PhoneList.h"
#include "../MCAL/UART/UART_Interface.h"
#include "../Service/debug_log.h"
#include "../Service/TopWayLCD_Interface.h"

// Buffer for terminal input
u8 inputBuffer[64];
// List to store phone numbers
List phoneList;
// Flag to control the main loop
u8 exitFlag = 0;
// Phone list types
#define CALLING_LIST 1
#define SMS_LIST 2

// Function prototypes
void initializeSystem(void);
void displayMenu(void);
void handleUserInput(void);
u8 getTerminalInput(u8* buffer, u8 maxSize);
void addPhoneNumber(void);
void removePhoneNumber(void);
void displayPhoneNumbers(void);
void validatePhoneNumber(void);
u8 isValidPhoneNumber(const u8* phoneNumber);
void printListToTerminal(List* list);

// Main function
int main(void) {
    // Initialize necessary hardware and data structures
    initializeSystem();
    
    // Main loop
    while (!exitFlag) {
        displayMenu();
        handleUserInput();
    }
    
    return 0;
}

// Initialize the system
void initializeSystem(void) {
    // Initialize UART
    UART_voidInit();
    UART_SendStringSync("\r\n\r\n*** Phone List Testing Utility ***\r\n");
    
    // Initialize debug logging
    DEBUG_LogInit(LOG_LEVEL_DEBUG);
    
    // Initialize LCD (our stub implementation)
    LCD_Init();
    
    // Initialize phone list
    CreateList(&phoneList);
    
    // Read existing phone list from EEPROM
    ReadListFromEEPROM(&phoneList);
    
    UART_SendStringSync("System initialized. EEPROM data loaded.\r\n");
    DEBUG_LogInfo("Phone list test utility started");
}

// Display the menu options
void displayMenu(void) {
    UART_SendStringSync("\r\n------------------------------\r\n");
    UART_SendStringSync("1. Add a phone number\r\n");
    UART_SendStringSync("2. Remove a phone number\r\n");
    UART_SendStringSync("3. Display all phone numbers\r\n");
    UART_SendStringSync("4. Validate a phone number\r\n");
    UART_SendStringSync("5. Exit\r\n");
    UART_SendStringSync("Select an option (1-5): ");
}

// Handle the user's menu selection
void handleUserInput(void) {
    getTerminalInput(inputBuffer, sizeof(inputBuffer) - 1);
    
    // Process input
    switch (inputBuffer[0]) {
        case '1':
            addPhoneNumber();
            break;
        case '2':
            removePhoneNumber();
            break;
        case '3':
            displayPhoneNumbers();
            break;
        case '4':
            validatePhoneNumber();
            break;
        case '5':
            UART_SendStringSync("Exiting...\r\n");
            exitFlag = 1;
            break;
        default:
            UART_SendStringSync("Invalid option. Please try again.\r\n");
            break;
    }
}

// Read a line of input from the terminal
u8 getTerminalInput(u8* buffer, u8 maxSize) {
    u8 i = 0;
    u8 c;
    
    // Clear the buffer
    memset(buffer, 0, maxSize);
    
    // Read characters until newline or max size
    while (i < maxSize - 1) {
        c = UART_u8ReadData();
        
        // Echo the character
        UART_voidWriteData(c);
        
        // Handle backspace
        if (c == '\b' || c == 127) {
            if (i > 0) {
                i--;
                // Clear the character on terminal (backspace, space, backspace)
                UART_voidWriteData('\b');
                UART_voidWriteData(' ');
                UART_voidWriteData('\b');
            }
            continue;
        }
        
        // Handle enter/return
        if (c == '\r' || c == '\n') {
            UART_SendStringSync("\r\n");
            break;
        }
        
        // Store character
        buffer[i++] = c;
    }
    
    buffer[i] = '\0';
    return i;
}

// Add a phone number to the list
void addPhoneNumber(void) {
    u8 listType;
    
    UART_SendStringSync("Enter phone number to add: ");
    getTerminalInput(inputBuffer, sizeof(inputBuffer) - 1);
    
    // Validate the phone number
    if (!isValidPhoneNumber(inputBuffer)) {
        UART_SendStringSync("Invalid phone number. Must start with 01 and be 11 digits.\r\n");
        return;
    }
    
    UART_SendStringSync("Select list type (1=Calling, 2=SMS): ");
    getTerminalInput(inputBuffer + 20, sizeof(inputBuffer) - 21); // Use a different part of the buffer
    
    // Convert input to number
    listType = inputBuffer[20] - '0';
    
    if (listType != CALLING_LIST && listType != SMS_LIST) {
        UART_SendStringSync("Invalid list type. Must be 1 or 2.\r\n");
        return;
    }
    
    // Add to the list
    AddNodeAtLast(&phoneList, inputBuffer, listType);
    
    // Save to EEPROM
    StoreListToEEPROM(&phoneList);
    
    UART_SendStringSync("Phone number added successfully and saved to EEPROM.\r\n");
    DEBUG_LogInfo("Phone number added to list and EEPROM");
}

// Remove a phone number from the list
void removePhoneNumber(void) {
    UART_SendStringSync("Enter phone number to remove: ");
    getTerminalInput(inputBuffer, sizeof(inputBuffer) - 1);
    
    // Validate the phone number format
    if (!isValidPhoneNumber(inputBuffer)) {
        UART_SendStringSync("Invalid phone number format.\r\n");
        return;
    }
    
    // Remove from the list
    Delete(inputBuffer, &phoneList);
    
    // Save to EEPROM
    StoreListToEEPROM(&phoneList);
    
    UART_SendStringSync("Phone number removed and EEPROM updated.\r\n");
    DEBUG_LogInfo("Phone number removed from list and EEPROM");
}

// Display all phone numbers in the list
void displayPhoneNumbers(void) {
    UART_SendStringSync("\r\n--- Phone Numbers List ---\r\n");
    
    if (phoneList.size == 0) {
        UART_SendStringSync("The phone list is empty.\r\n");
        return;
    }
    
    printListToTerminal(&phoneList);
}

// Validate a phone number without adding it
void validatePhoneNumber(void) {
    UART_SendStringSync("Enter phone number to validate: ");
    getTerminalInput(inputBuffer, sizeof(inputBuffer) - 1);
    
    if (isValidPhoneNumber(inputBuffer)) {
        UART_SendStringSync("Phone number is valid.\r\n");
    } else {
        UART_SendStringSync("Phone number is invalid. Must start with 01 and be 11 digits.\r\n");
    }
}

// Check if a phone number is valid
u8 isValidPhoneNumber(const u8* phoneNumber) {
    u8 length = 0;
    
    // Check if number starts with "01"
    if (phoneNumber[0] != '0' || phoneNumber[1] != '1')
        return 0;
    
    // Count the length
    while (phoneNumber[length] != '\0') {
        length++;
    }
    
    // Check if length is 11 digits
    return (length == 11);
}

// Print the list to the terminal
void printListToTerminal(List* list) {
    Node* current = list->Head;
    u8 index = 1;
    u8 callingCount = 0;
    u8 smsCount = 0;
    
    UART_SendStringSync("\r\n--- CALLING LIST ---\r\n");
    
    // Print calling list first
    current = list->Head;
    while (current != NULL) {
        if (current->SMSCALL == CALLING_LIST) {
            // Format string like: "1. 01150020317"
            UART_voidWriteData('0' + index++);
            UART_SendStringSync(". ");
            UART_SendStringSync(current->value);
            UART_SendStringSync("\r\n");
            callingCount++;
        }
        current = current->Next;
    }
    
    if (callingCount == 0) {
        UART_SendStringSync("No numbers in calling list.\r\n");
    }
    
    UART_SendStringSync("\r\n--- SMS LIST ---\r\n");
    
    // Print SMS list
    current = list->Head;
    index = 1;
    while (current != NULL) {
        if (current->SMSCALL == SMS_LIST) {
            // Format string like: "1. 01150020317"
            UART_voidWriteData('0' + index++);
            UART_SendStringSync(". ");
            UART_SendStringSync(current->value);
            UART_SendStringSync("\r\n");
            smsCount++;
        }
        current = current->Next;
    }
    
    if (smsCount == 0) {
        UART_SendStringSync("No numbers in SMS list.\r\n");
    }
    
    // Print total count
    UART_SendStringSync("\r\nTotal: ");
    // Convert list->size to string and print
    {
        char sizeStr[10];
        u8 i = 0;
        u16 size = list->size;
        
        do {
            sizeStr[i++] = '0' + (size % 10);
            size /= 10;
        } while (size > 0);
        
        // Reverse the digits
        sizeStr[i] = '\0';
        for (u8 j = 0; j < i/2; j++) {
            char temp = sizeStr[j];
            sizeStr[j] = sizeStr[i-j-1];
            sizeStr[i-j-1] = temp;
        }
        
        UART_SendStringSync(sizeStr);
    }
    
    UART_SendStringSync(" numbers (");
    UART_voidWriteData('0' + callingCount);
    UART_SendStringSync(" calling, ");
    UART_voidWriteData('0' + smsCount);
    UART_SendStringSync(" SMS)\r\n");
}