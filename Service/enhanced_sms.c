/*
 * enhanced_sms.c
 *
 * Implementation of enhanced SMS service with retry mechanism
 */

#include "enhanced_sms.h"
#include "SIM_interface.h"
#include "../Service/debug_log.h"
#include <string.h>
#include <stdio.h>
#include "../MCAL/Delay/delay.h"

// Internal function prototypes
static ES_t sendSMSWithDelay(const u8* phoneNumber, const u8* message);
static void formatTimestamp(u8* buffer, u32 timestamp);
static u8 validateMessage(const u8* message);

// Initialize SMS service
void SMS_Init(void) {
    DEBUG_LogInfo("Enhanced SMS service initialized");
}

// Send SMS with retry mechanism
ES_t SMS_SendWithRetry(const u8* phoneNumber, const u8* message, SMSMessageType_t type, u8 priority) {
    if (!phoneNumber || !message) {
        DEBUG_LogError("SMS: Null pointer passed");
        return ES_NULL_POINTER;
    }
    
    if (!SMS_ValidatePhoneNumber(phoneNumber)) {
        DEBUG_LogError("SMS: Invalid phone number");
        return ES_ERROR;
    }
    
    if (!validateMessage(message)) {
        DEBUG_LogError("SMS: Invalid message content");
        return ES_ERROR;
    }
    
    DEBUG_LogInfo("Sending SMS with retry mechanism");
    DEBUG_LogMessageWithValue(LOG_LEVEL_INFO, "Phone", (const char*)phoneNumber);
    DEBUG_LogMessageWithValue(LOG_LEVEL_INFO, "Message", (const char*)message);
    
    // Attempt to send SMS with retries
    for (u8 attempt = 0; attempt < g_SystemConfig.alarmRetryCount; attempt++) {
        DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "SMS attempt", attempt + 1);
        
        ES_t result = sendSMSWithDelay(phoneNumber, message);
        
        if (result == ES_OK) {
            DEBUG_LogInfo("SMS sent successfully");
            return ES_OK;
        }
        
        DEBUG_LogWarning("SMS send failed, retrying...");
        
        // Delay before retry (except for last attempt)
        if (attempt < g_SystemConfig.alarmRetryCount - 1) {
            for (u16 i = 0; i < g_SystemConfig.alarmRetryDelayMs; i++) {
                MCAL_DelayMs(1); // Delay in milliseconds
            }
        }
    }
    
    DEBUG_LogError("SMS: All retry attempts exhausted");
    return ES_ERROR;
}

// Send temperature alarm SMS
ES_t SMS_SendTemperatureAlarm(const u8* phoneNumber, f32 temperature, f32 minThreshold, f32 maxThreshold) {
    u8 message[160];
    u8 timestamp[32];
    
    // Get current timestamp (simplified)
    formatTimestamp(timestamp, 0); // In real implementation, use actual timestamp
    
    snprintf((char*)message, sizeof(message),
        "TEMPERATURE ALARM!\n"
        "Current: %.1f°C\n"
        "Range: %.1f to %.1f°C\n"
        "Time: %s\n"
        "Action required immediately!",
        temperature, minThreshold, maxThreshold, timestamp);
    
    return SMS_SendWithRetry(phoneNumber, message, SMS_TYPE_TEMPERATURE_ALARM, 5);
}

// Send current alarm SMS
ES_t SMS_SendCurrentAlarm(const u8* phoneNumber, f32 current, f32 power, f32 maxThreshold) {
    u8 message[160];
    u8 timestamp[32];
    
    formatTimestamp(timestamp, 0);
    
    snprintf((char*)message, sizeof(message),
        "CURRENT ALARM!\n"
        "Current: %.1fA\n"
        "Power: %.1fW\n"
        "Max: %.1fA\n"
        "Time: %s\n"
        "Check system immediately!",
        current, power, maxThreshold, timestamp);
    
    return SMS_SendWithRetry(phoneNumber, message, SMS_TYPE_CURRENT_ALARM, 5);
}

// Send system status SMS
ES_t SMS_SendSystemStatus(const u8* phoneNumber, f32 temperature, f32 current, f32 power) {
    u8 message[160];
    u8 timestamp[32];
    
    formatTimestamp(timestamp, 0);
    
    snprintf((char*)message, sizeof(message),
        "System Status Report\n"
        "Temp: %.1f°C\n"
        "Current: %.1fA\n"
        "Power: %.1fW\n"
        "Time: %s\n"
        "All systems operational",
        temperature, current, power, timestamp);
    
    return SMS_SendWithRetry(phoneNumber, message, SMS_TYPE_SYSTEM_STATUS, 2);
}

// Format alarm message based on type
ES_t SMS_FormatAlarmMessage(u8* buffer, SMSMessageType_t type, f32 value1, f32 value2, f32 value3) {
    if (!buffer) return ES_NULL_POINTER;
    
    switch (type) {
        case SMS_TYPE_TEMPERATURE_ALARM:
            snprintf((char*)buffer, 160,
                "TEMP ALARM: %.1f°C (Range: %.1f-%.1f°C)",
                value1, value2, value3);
            break;
            
        case SMS_TYPE_CURRENT_ALARM:
            snprintf((char*)buffer, 160,
                "CURRENT ALARM: %.1fA, Power: %.1fW (Max: %.1fA)",
                value1, value2, value3);
            break;
            
        case SMS_TYPE_SYSTEM_STATUS:
            snprintf((char*)buffer, 160,
                "Status: T=%.1f°C, I=%.1fA, P=%.1fW",
                value1, value2, value3);
            break;
            
        default:
            strcpy((char*)buffer, "Unknown alarm type");
            return ES_ERROR;
    }
    
    return ES_OK;
}

// Validate phone number format
u8 SMS_ValidatePhoneNumber(const u8* phoneNumber) {
    if (!phoneNumber) return 0;
    
    u8 length = strlen((const char*)phoneNumber);
    
    // Check length (should be 11 digits for Egyptian numbers)
    if (length != 11) return 0;
    
    // Check if starts with "01"
    if (phoneNumber[0] != '0' || phoneNumber[1] != '1') return 0;
    
    // Check if all characters are digits
    for (u8 i = 0; i < length; i++) {
        if (phoneNumber[i] < '0' || phoneNumber[i] > '9') return 0;
    }
    
    return 1;
}

// Internal: Send SMS with proper delays
static ES_t sendSMSWithDelay(const u8* phoneNumber, const u8* message) {
    ES_t result;
    
    // Check if SIM module is ready
    if (SIM_estCheckIfReady() != ES_OK) {
        DEBUG_LogError("SIM module not ready");
        return ES_ERROR;
    }
    
    // Check network registration
    if (SIM_estCheckIfRegistered() != ES_OK) {
        DEBUG_LogError("SIM not registered to network");
        return ES_ERROR;
    }
    
    // Send SMS using existing SIM module function
    result = SIM_estSendSMS((u8*)phoneNumber, (u8*)message);
    
    if (result == ES_OK) {
        DEBUG_LogInfo("SMS sent successfully via SIM module");
    } else {
        DEBUG_LogError("Failed to send SMS via SIM module");
    }
    
    return result;
}

// Format timestamp string (simplified implementation)
static void formatTimestamp(u8* buffer, u32 timestamp) {
    // In a real implementation, this would format the actual timestamp
    // For now, we'll use a simple placeholder
    strcpy((char*)buffer, "00:00:00");
}

// Validate message content
static u8 validateMessage(const u8* message) {
    if (!message) return 0;
    
    u8 length = strlen((const char*)message);
    
    // Check message length (SMS limit is 160 characters)
    if (length == 0 || length > 160) return 0;
    
    // Check for valid characters (basic ASCII check)
    for (u8 i = 0; i < length; i++) {
        if (message[i] < 32 || message[i] > 126) {
            // Allow newline and carriage return
            if (message[i] != '\n' && message[i] != '\r') {
                return 0;
            }
        }
    }
    
    return 1;
}