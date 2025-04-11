/*
 * test_phone_list_stub.c
 *
 * Stub implementations of LCD and related functions for terminal-based testing
 */

#include "../Utilities/STD_TYPES.h"
#include "../MCAL/UART/UART_Interface.h"

// LCD stub functions
void LCD_SendString(u32 address, u8* str) {
    // Empty implementation - do nothing
}

void LCD_SendNum16(u32 address, u16 num) {
    // Empty implementation - do nothing
}

u16 LCD_GetNum16(u32 address) {
    // For testing purposes, return values that simulate user interaction
    
    // Simulate Add button press when requested by the caller
    static u8 addButtonPressed = 0;
    
    // Define addresses for various LCD controls
    #define Add 0x4000       // Add button address
    #define Remove 0x4001     // Remove button address
    #define L_type 0x5000     // List type address
    #define Clear_BUFF 0x6000 // Clear buffer address
    
    if (address == Add) {
        // Toggle the add button state for testing
        addButtonPressed = !addButtonPressed;
        return addButtonPressed;
    }
    else if (address == L_type && addButtonPressed) {
        // Return Calling_list (1) when Add is pressed
        addButtonPressed = 0; // Reset for next test
        return 1;
    }
    else if (address == Remove) {
        // Always return 0 for remove button (not testing remove now)
        return 0;
    }
    else if (address == Clear_BUFF) {
        // Always return 0 for clear buffer
        return 0;
    }
    
    return 0;
}

// Function to initialize LCD stub functionality
void LCD_Init(void) {
    // In a real system, this would initialize the LCD hardware
    // For our terminal test, we'll just send a message
    UART_SendStringSync("LCD stub initialized (terminal mode).\r\n");
}