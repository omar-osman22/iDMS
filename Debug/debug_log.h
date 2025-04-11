#ifndef DEBUG_LOG_H_
#define DEBUG_LOG_H_

#include "../Utilities/STD_TYPES.h"
#include "../MCAL/UART/UART_Interface.h"

#define DEBUG_MODE
// Define debug levels
typedef enum {
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel_t;

// Current log level (can be changed at runtime)
extern volatile LogLevel_t g_CurrentLogLevel;

// Initialize debug logger
void DEBUG_LogInit(LogLevel_t initialLevel);

// Log functions
#ifdef DEBUG_MODE
    #define DEBUG_LogError(message)    DEBUG_LogMessage(LOG_LEVEL_ERROR, "ERROR: " message)
    #define DEBUG_LogWarning(message)  DEBUG_LogMessage(LOG_LEVEL_WARNING, "WARNING: " message)
    #define DEBUG_LogInfo(message)     DEBUG_LogMessage(LOG_LEVEL_INFO, "INFO: " message)
    #define DEBUG_LogDebug(message)    DEBUG_LogMessage(LOG_LEVEL_DEBUG, "DEBUG: " message)
    
    // Log with variable
    #define DEBUG_LogValueInt(level, message, value) do { \
        char buffer[12]; \
        itoa(value, buffer, 10); \
        DEBUG_LogMessageWithValue(level, message, buffer); \
    } while(0)
    
    // Function to send the log message
    void DEBUG_LogMessage(LogLevel_t level, const char* message);
    void DEBUG_LogMessageWithValue(LogLevel_t level, const char* message, const char* value);
#else
    // When DEBUG_MODE is not defined, these macros do nothing
    #define DEBUG_LogInit(level)                    ((void)0)
    #define DEBUG_LogError(message)                 ((void)0)
    #define DEBUG_LogWarning(message)               ((void)0)
    #define DEBUG_LogInfo(message)                  ((void)0)
    #define DEBUG_LogDebug(message)                 ((void)0)
    #define DEBUG_LogValueInt(level, message, value) ((void)0)
#endif

#endif /* DEBUG_LOG_H_ */