/*
 * data_logger.c
 *
 * Implementation of data logging system
 */

#include "data_logger.h"
#include "../MCAL/EEPROM/INTERNAL_EEPROM.h"
#include "../Debug/debug_log.h"
#include <string.h>

// Global data logger instance
DataLogger_t g_DataLogger;

// Initialize data logger
void DATALOG_Init(void) {
    DEBUG_LogInfo("Initializing data logger");
    
    // Initialize logger state
    g_DataLogger.currentIndex = 0;
    g_DataLogger.entryCount = 0;
    g_DataLogger.nextTimestamp = 1; // Start with timestamp 1
    
    // Clear buffer
    memset(g_DataLogger.buffer, 0, sizeof(g_DataLogger.buffer));
    
    // Load existing data from EEPROM
    DATALOG_LoadFromEEPROM();
    
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Loaded log entries", g_DataLogger.entryCount);
    DEBUG_LogInfo("Data logger initialized");
}

// Log a sensor reading
void DATALOG_LogReading(f32 temperature, f32 current, f32 power, u8 alarmFlags) {
    DataLogEntry_t* entry = &g_DataLogger.buffer[g_DataLogger.currentIndex];
    
    // Fill entry data
    entry->timestamp = g_DataLogger.nextTimestamp++;
    entry->temperature = temperature;
    entry->current = current;
    entry->power = power;
    entry->alarmFlags = alarmFlags;
    
    // Update circular buffer pointers
    g_DataLogger.currentIndex = (g_DataLogger.currentIndex + 1) % MAX_LOG_ENTRIES;
    
    // Update entry count (max is MAX_LOG_ENTRIES for circular buffer)
    if (g_DataLogger.entryCount < MAX_LOG_ENTRIES) {
        g_DataLogger.entryCount++;
    }
    
    DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Logged entry", entry->timestamp);
    
    // Periodically save to EEPROM (every 10 entries to reduce wear)
    if (entry->timestamp % 10 == 0) {
        DATALOG_SaveToEEPROM();
    }
}

// Save data log to EEPROM
void DATALOG_SaveToEEPROM(void) {
    DEBUG_LogInfo("Saving data log to EEPROM");
    
    u16 address = LOG_EEPROM_START;
    
    // Save logger state
    EEPROM_voidWriteData(address++, g_DataLogger.currentIndex);
    EEPROM_voidWriteData(address++, g_DataLogger.entryCount);
    
    // Save next timestamp (4 bytes)
    EEPROM_voidWriteData(address++, (u8)(g_DataLogger.nextTimestamp & 0xFF));
    EEPROM_voidWriteData(address++, (u8)((g_DataLogger.nextTimestamp >> 8) & 0xFF));
    EEPROM_voidWriteData(address++, (u8)((g_DataLogger.nextTimestamp >> 16) & 0xFF));
    EEPROM_voidWriteData(address++, (u8)((g_DataLogger.nextTimestamp >> 24) & 0xFF));
    
    // Save all log entries
    for (u8 i = 0; i < MAX_LOG_ENTRIES; i++) {
        DataLogEntry_t* entry = &g_DataLogger.buffer[i];
        
        // Save timestamp (4 bytes)
        EEPROM_voidWriteData(address++, (u8)(entry->timestamp & 0xFF));
        EEPROM_voidWriteData(address++, (u8)((entry->timestamp >> 8) & 0xFF));
        EEPROM_voidWriteData(address++, (u8)((entry->timestamp >> 16) & 0xFF));
        EEPROM_voidWriteData(address++, (u8)((entry->timestamp >> 24) & 0xFF));
        
        // Save float values as raw bytes (simplified for AVR)
        u8* tempBytes = (u8*)&entry->temperature;
        u8* currentBytes = (u8*)&entry->current;
        u8* powerBytes = (u8*)&entry->power;
        
        for (u8 j = 0; j < 4; j++) {
            EEPROM_voidWriteData(address++, tempBytes[j]);
        }
        for (u8 j = 0; j < 4; j++) {
            EEPROM_voidWriteData(address++, currentBytes[j]);
        }
        for (u8 j = 0; j < 4; j++) {
            EEPROM_voidWriteData(address++, powerBytes[j]);
        }
        
        // Save alarm flags
        EEPROM_voidWriteData(address++, entry->alarmFlags);
    }
    
    DEBUG_LogInfo("Data log saved to EEPROM");
}

// Load data log from EEPROM
void DATALOG_LoadFromEEPROM(void) {
    DEBUG_LogInfo("Loading data log from EEPROM");
    
    u16 address = LOG_EEPROM_START;
    
    // Load logger state
    g_DataLogger.currentIndex = EEPROM_u8ReadData(address++);
    g_DataLogger.entryCount = EEPROM_u8ReadData(address++);
    
    // Load next timestamp (4 bytes)
    g_DataLogger.nextTimestamp = EEPROM_u8ReadData(address++);
    g_DataLogger.nextTimestamp |= ((u32)EEPROM_u8ReadData(address++)) << 8;
    g_DataLogger.nextTimestamp |= ((u32)EEPROM_u8ReadData(address++)) << 16;
    g_DataLogger.nextTimestamp |= ((u32)EEPROM_u8ReadData(address++)) << 24;
    
    // Validate loaded data
    if (g_DataLogger.currentIndex >= MAX_LOG_ENTRIES || 
        g_DataLogger.entryCount > MAX_LOG_ENTRIES ||
        g_DataLogger.nextTimestamp == 0xFFFFFFFF) {
        DEBUG_LogWarning("Invalid data log in EEPROM, resetting");
        g_DataLogger.currentIndex = 0;
        g_DataLogger.entryCount = 0;
        g_DataLogger.nextTimestamp = 1;
        return;
    }
    
    // Load all log entries
    for (u8 i = 0; i < MAX_LOG_ENTRIES; i++) {
        DataLogEntry_t* entry = &g_DataLogger.buffer[i];
        
        // Load timestamp (4 bytes)
        entry->timestamp = EEPROM_u8ReadData(address++);
        entry->timestamp |= ((u32)EEPROM_u8ReadData(address++)) << 8;
        entry->timestamp |= ((u32)EEPROM_u8ReadData(address++)) << 16;
        entry->timestamp |= ((u32)EEPROM_u8ReadData(address++)) << 24;
        
        // Load float values as raw bytes
        u8* tempBytes = (u8*)&entry->temperature;
        u8* currentBytes = (u8*)&entry->current;
        u8* powerBytes = (u8*)&entry->power;
        
        for (u8 j = 0; j < 4; j++) {
            tempBytes[j] = EEPROM_u8ReadData(address++);
        }
        for (u8 j = 0; j < 4; j++) {
            currentBytes[j] = EEPROM_u8ReadData(address++);
        }
        for (u8 j = 0; j < 4; j++) {
            powerBytes[j] = EEPROM_u8ReadData(address++);
        }
        
        // Load alarm flags
        entry->alarmFlags = EEPROM_u8ReadData(address++);
    }
    
    DEBUG_LogInfo("Data log loaded from EEPROM");
}

// Get a specific log entry by index
DataLogEntry_t* DATALOG_GetEntry(u8 index) {
    if (index >= g_DataLogger.entryCount) {
        return NULL;
    }
    
    // Calculate actual buffer index for circular buffer
    u8 actualIndex;
    if (g_DataLogger.entryCount < MAX_LOG_ENTRIES) {
        actualIndex = index;
    } else {
        actualIndex = (g_DataLogger.currentIndex + index) % MAX_LOG_ENTRIES;
    }
    
    return &g_DataLogger.buffer[actualIndex];
}

// Get number of log entries
u8 DATALOG_GetEntryCount(void) {
    return g_DataLogger.entryCount;
}

// Calculate statistics from logged data
void DATALOG_GetStatistics(f32* avgTemp, f32* maxTemp, f32* minTemp, 
                          f32* avgCurrent, f32* maxCurrent, f32* minCurrent) {
    if (g_DataLogger.entryCount == 0) {
        *avgTemp = *maxTemp = *minTemp = 0;
        *avgCurrent = *maxCurrent = *minCurrent = 0;
        return;
    }
    
    f32 tempSum = 0, currentSum = 0;
    f32 tempMax = -999, tempMin = 999;
    f32 currentMax = -999, currentMin = 999;
    
    for (u8 i = 0; i < g_DataLogger.entryCount; i++) {
        DataLogEntry_t* entry = DATALOG_GetEntry(i);
        if (entry) {
            tempSum += entry->temperature;
            currentSum += entry->current;
            
            if (entry->temperature > tempMax) tempMax = entry->temperature;
            if (entry->temperature < tempMin) tempMin = entry->temperature;
            if (entry->current > currentMax) currentMax = entry->current;
            if (entry->current < currentMin) currentMin = entry->current;
        }
    }
    
    *avgTemp = tempSum / g_DataLogger.entryCount;
    *maxTemp = tempMax;
    *minTemp = tempMin;
    *avgCurrent = currentSum / g_DataLogger.entryCount;
    *maxCurrent = currentMax;
    *minCurrent = currentMin;
}

// Print log to debug output
void DATALOG_PrintLog(void) {
    DEBUG_LogInfo("=== Data Log ===");
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Total entries", g_DataLogger.entryCount);
    
    for (u8 i = 0; i < g_DataLogger.entryCount && i < 10; i++) { // Print first 10 entries
        DataLogEntry_t* entry = DATALOG_GetEntry(i);
        if (entry) {
            DEBUG_LogValueInt(LOG_LEVEL_INFO, "Entry timestamp", entry->timestamp);
            DEBUG_LogValueInt(LOG_LEVEL_INFO, "Temp (x10)", (int)(entry->temperature * 10));
            DEBUG_LogValueInt(LOG_LEVEL_INFO, "Current (x100)", (int)(entry->current * 100));
            DEBUG_LogValueInt(LOG_LEVEL_INFO, "Power (x10)", (int)(entry->power * 10));
            DEBUG_LogValueInt(LOG_LEVEL_INFO, "Alarm flags", entry->alarmFlags);
        }
    }
    
    DEBUG_LogInfo("================");
}

// Clear all log data
void DATALOG_ClearLog(void) {
    DEBUG_LogInfo("Clearing data log");
    
    g_DataLogger.currentIndex = 0;
    g_DataLogger.entryCount = 0;
    g_DataLogger.nextTimestamp = 1;
    
    memset(g_DataLogger.buffer, 0, sizeof(g_DataLogger.buffer));
    
    DATALOG_SaveToEEPROM();
    
    DEBUG_LogInfo("Data log cleared");
}