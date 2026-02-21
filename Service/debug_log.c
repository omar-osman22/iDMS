#include "debug_log.h"
#include <string.h>
#include <avr/io.h>

// Set the default log level
volatile LogLevel_t g_CurrentLogLevel = LOG_LEVEL_INFO;

#ifdef DEBUG_MODE

// Helper function for string conversion
void reverse(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Integer to ASCII conversion
char* itoa(int num, char* str, int base) {
    int i = 0;
    int isNegative = 0;
    
    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    
    // Handle negative numbers
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }
    
    // Process digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    
    // If negative number
    if (isNegative)
        str[i++] = '-';
    
    str[i] = '\0'; // Null terminate string
    
    // Reverse the string
    reverse(str, i);
    
    return str;
}

// Initialize debug logger - sets up UART communication
void DEBUG_LogInit(LogLevel_t initialLevel) {
    g_CurrentLogLevel = initialLevel;
    
    // Use the standard UART for debug logs
    // Make sure this doesn't conflict with the UART used by LCD
    UART_voidInit();
    
    // Send an initial message to confirm logger is working
    UART_voidWriteString((u8*)"DEBUG LOGGER INITIALIZED\r\n");
}

// Send log message if level is appropriate
void DEBUG_LogMessage(LogLevel_t level, const char* message) {
    if (level <= g_CurrentLogLevel) {
        // Send message over UART - this will be visible during simulation
        // Output message with a visible prefix
        UART_voidWriteString((u8*)"[DEBUG] ");
        UART_voidWriteString((u8*)message);
        UART_voidWriteData('\r');
        UART_voidWriteData('\n');
    }
}

// Send log message with a value
void DEBUG_LogMessageWithValue(LogLevel_t level, const char* message, const char* value) {
    if (level <= g_CurrentLogLevel) {
        // Send message over UART with value
        UART_voidWriteString((u8*)"[DEBUG] ");
        UART_voidWriteString((u8*)message);
        UART_voidWriteString((u8*)": ");
        UART_voidWriteString((u8*)value);
        UART_voidWriteData('\r');
        UART_voidWriteData('\n');
    }
}

#endif /* DEBUG_MODE */