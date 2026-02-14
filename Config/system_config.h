/*
 * system_config.h
 *
 * System configuration management for Industrial Monitoring System (iDMS)
 * Provides centralized configuration with EEPROM persistence
 */

#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

#include "../Utilities/STD_TYPES.h"

// Configuration version for EEPROM compatibility
#define CONFIG_VERSION 0x01

// Configuration structure stored in EEPROM
typedef struct {
    u8 version;                     // Configuration version
    
    // Sensor Configuration
    f32 tempThresholdMax;          // Maximum temperature threshold
    f32 tempThresholdMin;          // Minimum temperature threshold
    f32 currentThresholdMax;       // Maximum current threshold
    f32 filterAlpha;               // IIR filter coefficient
    f32 outlierThreshold;          // Outlier detection threshold
    u16 calibrationSamples;        // Number of calibration samples
    
    // Alarm Configuration
    u32 alarmDelayMs;              // Delay between alarm messages
    u8 alarmRetryCount;            // Number of retry attempts for SMS
    u16 alarmRetryDelayMs;         // Delay between retries
    
    // Display Configuration
    u8 displayUpdateInterval;      // Display update interval in main loops
    u8 temperatureOffset;          // Temperature display offset
    
    // System Configuration
    u8 debugLogLevel;              // Debug logging level
    u8 powerSaveMode;              // Power saving mode enable
    u16 systemWatchdogTimeout;     // Watchdog timeout in seconds
    
    // Checksum for integrity verification
    u16 checksum;
} SystemConfig;

// Default configuration values
#define DEFAULT_TEMP_MAX           85.0f
#define DEFAULT_TEMP_MIN           -20.0f
#define DEFAULT_CURRENT_MAX        20.0f
#define DEFAULT_FILTER_ALPHA       0.15f
#define DEFAULT_OUTLIER_THRESHOLD  3.0f
#define DEFAULT_CALIBRATION_SAMPLES 100
#define DEFAULT_ALARM_DELAY        5000
#define DEFAULT_ALARM_RETRY_COUNT  3
#define DEFAULT_ALARM_RETRY_DELAY  2000
#define DEFAULT_DISPLAY_INTERVAL   1
#define DEFAULT_TEMP_OFFSET        50
#define DEFAULT_DEBUG_LEVEL        2
#define DEFAULT_POWER_SAVE         0
#define DEFAULT_WATCHDOG_TIMEOUT   30

// Global configuration instance
extern SystemConfig g_SystemConfig;

// Function prototypes
void CONFIG_Init(void);
void CONFIG_LoadDefaults(void);
void CONFIG_LoadFromEEPROM(void);
void CONFIG_SaveToEEPROM(void);
u8 CONFIG_ValidateChecksum(void);
void CONFIG_UpdateChecksum(void);
u8 CONFIG_SetTempThresholds(f32 min, f32 max);
u8 CONFIG_SetCurrentThreshold(f32 max);
u8 CONFIG_SetFilterParameters(f32 alpha, f32 outlierThreshold);
u8 CONFIG_SetAlarmParameters(u32 delayMs, u8 retryCount, u16 retryDelayMs);
void CONFIG_PrintCurrent(void);

#endif /* SYSTEM_CONFIG_H_ */