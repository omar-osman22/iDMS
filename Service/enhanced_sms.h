/*
 * enhanced_sms.h
 *
 * Enhanced SMS service with retry mechanism and better formatting
 */

#ifndef ENHANCED_SMS_H_
#define ENHANCED_SMS_H_

#include "../Utilities/STD_TYPES.h"
#include "../Utilities/errorStates.h"
#include "../Config/system_config.h"

// SMS message types
typedef enum {
    SMS_TYPE_TEMPERATURE_ALARM,
    SMS_TYPE_CURRENT_ALARM,
    SMS_TYPE_SYSTEM_STATUS,
    SMS_TYPE_CUSTOM
} SMSMessageType_t;

// SMS sending result
typedef enum {
    SMS_RESULT_SUCCESS,
    SMS_RESULT_FAILED,
    SMS_RESULT_RETRY_EXHAUSTED,
    SMS_RESULT_INVALID_NUMBER,
    SMS_RESULT_NETWORK_ERROR
} SMSResult_t;

// SMS message structure
typedef struct {
    u8 phoneNumber[12];        // Phone number to send to
    u8 message[160];           // SMS message content (max 160 chars)
    SMSMessageType_t type;     // Type of message
    u8 retryCount;             // Current retry count
    u32 timestamp;             // Timestamp of message creation
    u8 priority;               // Message priority (1-5, 5 being highest)
} SMSMessage_t;

// Function prototypes
ES_t SMS_SendWithRetry(const u8* phoneNumber, const u8* message, SMSMessageType_t type, u8 priority);
ES_t SMS_SendTemperatureAlarm(const u8* phoneNumber, f32 temperature, f32 minThreshold, f32 maxThreshold);
ES_t SMS_SendCurrentAlarm(const u8* phoneNumber, f32 current, f32 power, f32 maxThreshold);
ES_t SMS_SendSystemStatus(const u8* phoneNumber, f32 temperature, f32 current, f32 power);
ES_t SMS_FormatAlarmMessage(u8* buffer, SMSMessageType_t type, f32 value1, f32 value2, f32 value3);
u8 SMS_ValidatePhoneNumber(const u8* phoneNumber);
void SMS_Init(void);

#endif /* ENHANCED_SMS_H_ */