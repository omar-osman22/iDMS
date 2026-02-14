/*
 * system_config.c
 *
 * Implementation of system configuration management
 */

#include "system_config.h"
#include "../MCAL/EEPROM/INTERNAL_EEPROM.h"
#include "../Debug/debug_log.h"
#include <string.h>

// Global configuration instance
SystemConfig g_SystemConfig;

// EEPROM address for configuration storage
#define CONFIG_EEPROM_ADDRESS 0x200

// Calculate checksum for configuration integrity
static u16 calculateChecksum(const SystemConfig* config) {
    u16 checksum = 0;
    const u8* data = (const u8*)config;
    
    // Calculate checksum for all fields except the checksum field itself
    for (u16 i = 0; i < sizeof(SystemConfig) - sizeof(u16); i++) {
        checksum += data[i];
    }
    
    return checksum;
}

// Initialize configuration system
void CONFIG_Init(void) {
    DEBUG_LogInfo("Initializing system configuration");
    
    CONFIG_LoadFromEEPROM();
    
    // Validate configuration
    if (g_SystemConfig.version != CONFIG_VERSION || !CONFIG_ValidateChecksum()) {
        DEBUG_LogWarning("Invalid configuration in EEPROM, loading defaults");
        CONFIG_LoadDefaults();
        CONFIG_SaveToEEPROM();
    }
    
    DEBUG_LogInfo("Configuration system initialized");
    CONFIG_PrintCurrent();
}

// Load default configuration values
void CONFIG_LoadDefaults(void) {
    g_SystemConfig.version = CONFIG_VERSION;
    
    // Sensor Configuration
    g_SystemConfig.tempThresholdMax = DEFAULT_TEMP_MAX;
    g_SystemConfig.tempThresholdMin = DEFAULT_TEMP_MIN;
    g_SystemConfig.currentThresholdMax = DEFAULT_CURRENT_MAX;
    g_SystemConfig.filterAlpha = DEFAULT_FILTER_ALPHA;
    g_SystemConfig.outlierThreshold = DEFAULT_OUTLIER_THRESHOLD;
    g_SystemConfig.calibrationSamples = DEFAULT_CALIBRATION_SAMPLES;
    
    // Alarm Configuration
    g_SystemConfig.alarmDelayMs = DEFAULT_ALARM_DELAY;
    g_SystemConfig.alarmRetryCount = DEFAULT_ALARM_RETRY_COUNT;
    g_SystemConfig.alarmRetryDelayMs = DEFAULT_ALARM_RETRY_DELAY;
    
    // Display Configuration
    g_SystemConfig.displayUpdateInterval = DEFAULT_DISPLAY_INTERVAL;
    g_SystemConfig.temperatureOffset = DEFAULT_TEMP_OFFSET;
    
    // System Configuration
    g_SystemConfig.debugLogLevel = DEFAULT_DEBUG_LEVEL;
    g_SystemConfig.powerSaveMode = DEFAULT_POWER_SAVE;
    g_SystemConfig.systemWatchdogTimeout = DEFAULT_WATCHDOG_TIMEOUT;
    
    CONFIG_UpdateChecksum();
    
    DEBUG_LogInfo("Default configuration loaded");
}

// Load configuration from EEPROM
void CONFIG_LoadFromEEPROM(void) {
    DEBUG_LogInfo("Loading configuration from EEPROM");
    
    // Read configuration from EEPROM
    for (u16 i = 0; i < sizeof(SystemConfig); i++) {
        ((u8*)&g_SystemConfig)[i] = EEPROM_u8ReadData(CONFIG_EEPROM_ADDRESS + i);
    }
    
    DEBUG_LogInfo("Configuration loaded from EEPROM");
}

// Save configuration to EEPROM
void CONFIG_SaveToEEPROM(void) {
    DEBUG_LogInfo("Saving configuration to EEPROM");
    
    CONFIG_UpdateChecksum();
    
    // Write configuration to EEPROM
    for (u16 i = 0; i < sizeof(SystemConfig); i++) {
        EEPROM_voidWriteData(CONFIG_EEPROM_ADDRESS + i, ((u8*)&g_SystemConfig)[i]);
    }
    
    DEBUG_LogInfo("Configuration saved to EEPROM");
}

// Validate configuration checksum
u8 CONFIG_ValidateChecksum(void) {
    u16 calculatedChecksum = calculateChecksum(&g_SystemConfig);
    return (calculatedChecksum == g_SystemConfig.checksum);
}

// Update configuration checksum
void CONFIG_UpdateChecksum(void) {
    g_SystemConfig.checksum = calculateChecksum(&g_SystemConfig);
}

// Set temperature thresholds with validation
u8 CONFIG_SetTempThresholds(f32 min, f32 max) {
    if (min >= max || min < -50.0f || max > 150.0f) {
        DEBUG_LogError("Invalid temperature thresholds");
        return 0;
    }
    
    g_SystemConfig.tempThresholdMin = min;
    g_SystemConfig.tempThresholdMax = max;
    CONFIG_SaveToEEPROM();
    
    DEBUG_LogInfo("Temperature thresholds updated");
    return 1;
}

// Set current threshold with validation
u8 CONFIG_SetCurrentThreshold(f32 max) {
    if (max <= 0 || max > 100.0f) {
        DEBUG_LogError("Invalid current threshold");
        return 0;
    }
    
    g_SystemConfig.currentThresholdMax = max;
    CONFIG_SaveToEEPROM();
    
    DEBUG_LogInfo("Current threshold updated");
    return 1;
}

// Set filter parameters with validation
u8 CONFIG_SetFilterParameters(f32 alpha, f32 outlierThreshold) {
    if (alpha <= 0 || alpha >= 1.0f || outlierThreshold <= 0 || outlierThreshold > 10.0f) {
        DEBUG_LogError("Invalid filter parameters");
        return 0;
    }
    
    g_SystemConfig.filterAlpha = alpha;
    g_SystemConfig.outlierThreshold = outlierThreshold;
    CONFIG_SaveToEEPROM();
    
    DEBUG_LogInfo("Filter parameters updated");
    return 1;
}

// Set alarm parameters with validation
u8 CONFIG_SetAlarmParameters(u32 delayMs, u8 retryCount, u16 retryDelayMs) {
    if (delayMs < 1000 || delayMs > 60000 || retryCount > 10 || retryDelayMs < 500) {
        DEBUG_LogError("Invalid alarm parameters");
        return 0;
    }
    
    g_SystemConfig.alarmDelayMs = delayMs;
    g_SystemConfig.alarmRetryCount = retryCount;
    g_SystemConfig.alarmRetryDelayMs = retryDelayMs;
    CONFIG_SaveToEEPROM();
    
    DEBUG_LogInfo("Alarm parameters updated");
    return 1;
}

// Print current configuration (for debugging)
void CONFIG_PrintCurrent(void) {
    DEBUG_LogInfo("=== Current System Configuration ===");
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Config Version", g_SystemConfig.version);
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Temp Max (x10)", (int)(g_SystemConfig.tempThresholdMax * 10));
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Temp Min (x10)", (int)(g_SystemConfig.tempThresholdMin * 10));
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Current Max (x10)", (int)(g_SystemConfig.currentThresholdMax * 10));
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Filter Alpha (x100)", (int)(g_SystemConfig.filterAlpha * 100));
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Alarm Delay (ms)", g_SystemConfig.alarmDelayMs);
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Alarm Retry Count", g_SystemConfig.alarmRetryCount);
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Debug Level", g_SystemConfig.debugLogLevel);
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Checksum", g_SystemConfig.checksum);
    DEBUG_LogInfo("=====================================");
}