/*
 * error_handler.c
 *
 * Implementation of centralized error handling system
 */

#include "error_handler.h"
#include "../Debug/debug_log.h"
#include "../MCAL/EEPROM/INTERNAL_EEPROM.h"
#include "../Service/enhanced_sms.h"
#include "../Service/watchdog_timer.h"
#include <string.h>

// Global error handler instance
ErrorHandler_t g_ErrorHandler;

// Initialize error handling system
void ERROR_Init(void) {
    DEBUG_LogInfo("Initializing error handler");
    
    // Initialize error handler state
    memset(&g_ErrorHandler, 0, sizeof(ErrorHandler_t));
    g_ErrorHandler.currentIndex = 0;
    g_ErrorHandler.errorCount = 0;
    g_ErrorHandler.totalErrors = 0;
    g_ErrorHandler.criticalErrorActive = 0;
    
    // Load existing error log from EEPROM
    ERROR_LoadFromEEPROM();
    
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Loaded error records", g_ErrorHandler.errorCount);
    DEBUG_LogInfo("Error handler initialized");
}

// Report a new error
void ERROR_Report(u16 errorCode, ErrorSeverity_t severity, ErrorCategory_t category, 
                 const u8* additionalData, const char* description) {
    
    DEBUG_LogValueInt(LOG_LEVEL_ERROR, "Error reported", errorCode);
    DEBUG_LogMessageWithValue(LOG_LEVEL_ERROR, "Description", description ? description : "No description");
    
    // Create error record
    ErrorRecord_t* error = &g_ErrorHandler.errorLog[g_ErrorHandler.currentIndex];
    error->errorCode = errorCode;
    error->severity = severity;
    error->category = category;
    error->timestamp = WATCHDOG_GetSystemUptime();
    error->description = description;
    
    // Copy additional data if provided
    if (additionalData) {
        memcpy(error->additionalData, additionalData, 4);
    } else {
        memset(error->additionalData, 0, 4);
    }
    
    // Update circular buffer pointers
    g_ErrorHandler.currentIndex = (g_ErrorHandler.currentIndex + 1) % MAX_ERROR_RECORDS;
    if (g_ErrorHandler.errorCount < MAX_ERROR_RECORDS) {
        g_ErrorHandler.errorCount++;
    }
    g_ErrorHandler.totalErrors++;
    
    // Handle critical errors
    if (severity == ERROR_SEVERITY_CRITICAL) {
        g_ErrorHandler.criticalErrorActive = 1;
        DEBUG_LogError("CRITICAL ERROR DETECTED - SYSTEM MAY BE UNSTABLE");
        
        // In a real system, this might trigger immediate actions:
        // - Emergency shutdown
        // - Safe mode activation
        // - Immediate notification to operators
    }
    
    // Attempt automatic recovery for known errors
    ERROR_HandleRecovery(errorCode);
    
    // Save error log periodically
    if (g_ErrorHandler.totalErrors % 5 == 0) {
        ERROR_SaveToEEPROM();
    }
}

// Clear a specific error
void ERROR_Clear(u16 errorCode) {
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Clearing error", errorCode);
    
    for (u8 i = 0; i < g_ErrorHandler.errorCount; i++) {
        if (g_ErrorHandler.errorLog[i].errorCode == errorCode) {
            // Mark error as cleared by setting error code to 0
            g_ErrorHandler.errorLog[i].errorCode = 0;
            DEBUG_LogInfo("Error cleared");
            return;
        }
    }
    
    DEBUG_LogWarning("Error not found in log");
}

// Clear all errors
void ERROR_ClearAll(void) {
    DEBUG_LogInfo("Clearing all errors");
    
    memset(&g_ErrorHandler.errorLog, 0, sizeof(g_ErrorHandler.errorLog));
    g_ErrorHandler.currentIndex = 0;
    g_ErrorHandler.errorCount = 0;
    g_ErrorHandler.criticalErrorActive = 0;
    
    ERROR_SaveToEEPROM();
    DEBUG_LogInfo("All errors cleared");
}

// Get error count
u8 ERROR_GetCount(void) {
    return g_ErrorHandler.errorCount;
}

// Get specific error record
ErrorRecord_t* ERROR_GetRecord(u8 index) {
    if (index >= g_ErrorHandler.errorCount) {
        return NULL;
    }
    
    // Calculate actual buffer index for circular buffer
    u8 actualIndex;
    if (g_ErrorHandler.errorCount < MAX_ERROR_RECORDS) {
        actualIndex = index;
    } else {
        actualIndex = (g_ErrorHandler.currentIndex + index) % MAX_ERROR_RECORDS;
    }
    
    return &g_ErrorHandler.errorLog[actualIndex];
}

// Print error log to debug output
void ERROR_PrintLog(void) {
    DEBUG_LogInfo("=== Error Log ===");
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Total errors logged", g_ErrorHandler.totalErrors);
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Current error count", g_ErrorHandler.errorCount);
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Critical error active", g_ErrorHandler.criticalErrorActive);
    
    for (u8 i = 0; i < g_ErrorHandler.errorCount && i < 10; i++) { // Print first 10 errors
        ErrorRecord_t* error = ERROR_GetRecord(i);
        if (error && error->errorCode != 0) {
            DEBUG_LogValueInt(LOG_LEVEL_INFO, "Error code", error->errorCode);
            DEBUG_LogValueInt(LOG_LEVEL_INFO, "Severity", error->severity);
            DEBUG_LogValueInt(LOG_LEVEL_INFO, "Category", error->category);
            DEBUG_LogValueInt(LOG_LEVEL_INFO, "Timestamp", error->timestamp);
            if (error->description) {
                DEBUG_LogMessageWithValue(LOG_LEVEL_INFO, "Description", error->description);
            }
        }
    }
    
    DEBUG_LogInfo("==================");
}

// Save error log to EEPROM
void ERROR_SaveToEEPROM(void) {
    DEBUG_LogInfo("Saving error log to EEPROM");
    
    u16 address = ERROR_LOG_EEPROM_ADDRESS;
    
    // Save error handler state
    EEPROM_voidWriteData(address++, g_ErrorHandler.currentIndex);
    EEPROM_voidWriteData(address++, g_ErrorHandler.errorCount);
    
    // Save total errors (4 bytes)
    EEPROM_voidWriteData(address++, (u8)(g_ErrorHandler.totalErrors & 0xFF));
    EEPROM_voidWriteData(address++, (u8)((g_ErrorHandler.totalErrors >> 8) & 0xFF));
    EEPROM_voidWriteData(address++, (u8)((g_ErrorHandler.totalErrors >> 16) & 0xFF));
    EEPROM_voidWriteData(address++, (u8)((g_ErrorHandler.totalErrors >> 24) & 0xFF));
    
    EEPROM_voidWriteData(address++, g_ErrorHandler.criticalErrorActive);
    
    // Save error records (simplified - save only essential data)
    for (u8 i = 0; i < MAX_ERROR_RECORDS; i++) {
        ErrorRecord_t* error = &g_ErrorHandler.errorLog[i];
        
        // Save error code (2 bytes)
        EEPROM_voidWriteData(address++, (u8)(error->errorCode & 0xFF));
        EEPROM_voidWriteData(address++, (u8)((error->errorCode >> 8) & 0xFF));
        
        // Save severity and category
        EEPROM_voidWriteData(address++, error->severity);
        EEPROM_voidWriteData(address++, error->category);
        
        // Save timestamp (4 bytes)
        EEPROM_voidWriteData(address++, (u8)(error->timestamp & 0xFF));
        EEPROM_voidWriteData(address++, (u8)((error->timestamp >> 8) & 0xFF));
        EEPROM_voidWriteData(address++, (u8)((error->timestamp >> 16) & 0xFF));
        EEPROM_voidWriteData(address++, (u8)((error->timestamp >> 24) & 0xFF));
        
        // Save additional data
        for (u8 j = 0; j < 4; j++) {
            EEPROM_voidWriteData(address++, error->additionalData[j]);
        }
    }
    
    DEBUG_LogInfo("Error log saved to EEPROM");
}

// Load error log from EEPROM
void ERROR_LoadFromEEPROM(void) {
    DEBUG_LogInfo("Loading error log from EEPROM");
    
    u16 address = ERROR_LOG_EEPROM_ADDRESS;
    
    // Load error handler state
    g_ErrorHandler.currentIndex = EEPROM_u8ReadData(address++);
    g_ErrorHandler.errorCount = EEPROM_u8ReadData(address++);
    
    // Load total errors (4 bytes)
    g_ErrorHandler.totalErrors = EEPROM_u8ReadData(address++);
    g_ErrorHandler.totalErrors |= ((u32)EEPROM_u8ReadData(address++)) << 8;
    g_ErrorHandler.totalErrors |= ((u32)EEPROM_u8ReadData(address++)) << 16;
    g_ErrorHandler.totalErrors |= ((u32)EEPROM_u8ReadData(address++)) << 24;
    
    g_ErrorHandler.criticalErrorActive = EEPROM_u8ReadData(address++);
    
    // Validate loaded data
    if (g_ErrorHandler.currentIndex >= MAX_ERROR_RECORDS || 
        g_ErrorHandler.errorCount > MAX_ERROR_RECORDS) {
        DEBUG_LogWarning("Invalid error log in EEPROM, resetting");
        ERROR_ClearAll();
        return;
    }
    
    // Load error records
    for (u8 i = 0; i < MAX_ERROR_RECORDS; i++) {
        ErrorRecord_t* error = &g_ErrorHandler.errorLog[i];
        
        // Load error code (2 bytes)
        error->errorCode = EEPROM_u8ReadData(address++);
        error->errorCode |= ((u16)EEPROM_u8ReadData(address++)) << 8;
        
        // Load severity and category
        error->severity = EEPROM_u8ReadData(address++);
        error->category = EEPROM_u8ReadData(address++);
        
        // Load timestamp (4 bytes)
        error->timestamp = EEPROM_u8ReadData(address++);
        error->timestamp |= ((u32)EEPROM_u8ReadData(address++)) << 8;
        error->timestamp |= ((u32)EEPROM_u8ReadData(address++)) << 16;
        error->timestamp |= ((u32)EEPROM_u8ReadData(address++)) << 24;
        
        // Load additional data
        for (u8 j = 0; j < 4; j++) {
            error->additionalData[j] = EEPROM_u8ReadData(address++);
        }
        
        // Description pointer will be NULL after loading (not stored in EEPROM)
        error->description = NULL;
    }
    
    DEBUG_LogInfo("Error log loaded from EEPROM");
}

// Check if there are critical errors
u8 ERROR_HasCriticalErrors(void) {
    return g_ErrorHandler.criticalErrorActive;
}

// Get error count by category
u8 ERROR_GetErrorsByCategory(ErrorCategory_t category) {
    u8 count = 0;
    
    for (u8 i = 0; i < g_ErrorHandler.errorCount; i++) {
        ErrorRecord_t* error = ERROR_GetRecord(i);
        if (error && error->errorCode != 0 && error->category == category) {
            count++;
        }
    }
    
    return count;
}

// Handle automatic error recovery
ES_t ERROR_HandleRecovery(u16 errorCode) {
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Attempting recovery for error", errorCode);
    
    switch (errorCode) {
        case ERROR_SENSOR_READ_FAILED:
            // Try to reinitialize sensors
            DEBUG_LogInfo("Recovery: Reinitializing current sensor");
            CT_vInit();
            return ES_OK;
            
        case ERROR_SMS_SEND_FAILED:
            // Could try to reinitialize SIM module
            DEBUG_LogInfo("Recovery: SMS send failed - checking network");
            // In real implementation: reinitialize SIM module
            return ES_OK;
            
        case ERROR_EEPROM_WRITE_FAILED:
            // Try alternative storage or mark as read-only mode
            DEBUG_LogWarning("Recovery: EEPROM write failed - entering read-only mode");
            return ES_ERROR;
            
        case ERROR_WATCHDOG_TIMEOUT:
            // Log timeout and reset affected task
            DEBUG_LogError("Recovery: Watchdog timeout - resetting task");
            return ES_OK;
            
        default:
            DEBUG_LogInfo("No automatic recovery available");
            return ES_NOK;
    }
}