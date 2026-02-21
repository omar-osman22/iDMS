/*
 * data_logger.h
 *
 * Simple data logging system for sensor readings with EEPROM storage
 */

#ifndef DATA_LOGGER_H_
#define DATA_LOGGER_H_

#include "../Utilities/STD_TYPES.h"
#include "../Config/eeprom_map.h"

// Data log entry structure
typedef struct {
    u32 timestamp;          // Unix timestamp (simplified)
    f32 temperature;        // Temperature reading
    f32 current;           // Current reading
    f32 power;             // Power reading
    u8 alarmFlags;         // Alarm status flags
} DataLogEntry_t;

// Configuration constants
#define MAX_LOG_ENTRIES     EEPROM_DATALOG_MAX_ENTRIES    // Maximum entries in circular buffer
#define LOG_EEPROM_START    EEPROM_DATALOG_BASE // EEPROM address for log storage
#define LOG_ENTRY_SIZE      sizeof(DataLogEntry_t)

// Alarm flag bits
#define ALARM_FLAG_TEMP_HIGH    (1 << 0)
#define ALARM_FLAG_TEMP_LOW     (1 << 1)
#define ALARM_FLAG_CURRENT_HIGH (1 << 2)
#define ALARM_FLAG_SYSTEM_OK    (1 << 3)

// Global data logger state
typedef struct {
    u8 currentIndex;        // Current position in circular buffer
    u8 entryCount;         // Number of entries stored
    u32 nextTimestamp;     // Next timestamp to use
    DataLogEntry_t buffer[MAX_LOG_ENTRIES]; // In-memory circular buffer
} DataLogger_t;

extern DataLogger_t g_DataLogger;

// Function prototypes
void DATALOG_Init(void);
void DATALOG_LogReading(f32 temperature, f32 current, f32 power, u8 alarmFlags);
void DATALOG_SaveToEEPROM(void);
void DATALOG_LoadFromEEPROM(void);
DataLogEntry_t* DATALOG_GetEntry(u8 index);
u8 DATALOG_GetEntryCount(void);
void DATALOG_GetStatistics(f32* avgTemp, f32* maxTemp, f32* minTemp, 
                          f32* avgCurrent, f32* maxCurrent, f32* minCurrent);
void DATALOG_PrintLog(void);
void DATALOG_ClearLog(void);

#endif /* DATA_LOGGER_H_ */