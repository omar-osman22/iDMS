/*
 * error_handler.h
 *
 * Centralized error handling system for improved reliability
 */

#ifndef ERROR_HANDLER_H_
#define ERROR_HANDLER_H_

#include "../Utilities/STD_TYPES.h"
#include "../Utilities/errorStates.h"

// Error severity levels
typedef enum {
    ERROR_SEVERITY_INFO,
    ERROR_SEVERITY_WARNING,
    ERROR_SEVERITY_ERROR,
    ERROR_SEVERITY_CRITICAL
} ErrorSeverity_t;

// Error categories
typedef enum {
    ERROR_CATEGORY_SENSOR,
    ERROR_CATEGORY_COMMUNICATION,
    ERROR_CATEGORY_MEMORY,
    ERROR_CATEGORY_CONFIGURATION,
    ERROR_CATEGORY_SYSTEM,
    ERROR_CATEGORY_APPLICATION
} ErrorCategory_t;

// Error codes
#define ERROR_SENSOR_READ_FAILED        0x1001
#define ERROR_SENSOR_CALIBRATION_FAILED 0x1002
#define ERROR_SENSOR_OUTLIER_DETECTED   0x1003

#define ERROR_SMS_SEND_FAILED           0x2001
#define ERROR_SMS_NETWORK_UNAVAILABLE   0x2002
#define ERROR_SMS_INVALID_NUMBER        0x2003

#define ERROR_EEPROM_READ_FAILED        0x3001
#define ERROR_EEPROM_WRITE_FAILED       0x3002
#define ERROR_EEPROM_CHECKSUM_ERROR     0x3003

#define ERROR_CONFIG_INVALID            0x4001
#define ERROR_CONFIG_SAVE_FAILED        0x4002

#define ERROR_WATCHDOG_TIMEOUT          0x5001
#define ERROR_SYSTEM_OVERLOAD           0x5002

#define ERROR_PHONE_LIST_FULL           0x6001
#define ERROR_INVALID_INPUT             0x6002

// Error record structure
typedef struct {
    u16 errorCode;
    ErrorSeverity_t severity;
    ErrorCategory_t category;
    u32 timestamp;
    u8 additionalData[4];  // Context-specific data
    const char* description;
} ErrorRecord_t;

// Error handler configuration
#define MAX_ERROR_RECORDS 20
#define ERROR_LOG_EEPROM_ADDRESS 0x400

// Global error handler state
typedef struct {
    ErrorRecord_t errorLog[MAX_ERROR_RECORDS];
    u8 currentIndex;
    u8 errorCount;
    u32 totalErrors;
    u8 criticalErrorActive;
} ErrorHandler_t;

extern ErrorHandler_t g_ErrorHandler;

// Function prototypes
void ERROR_Init(void);
void ERROR_Report(u16 errorCode, ErrorSeverity_t severity, ErrorCategory_t category, 
                 const u8* additionalData, const char* description);
void ERROR_Clear(u16 errorCode);
void ERROR_ClearAll(void);
u8 ERROR_GetCount(void);
ErrorRecord_t* ERROR_GetRecord(u8 index);
void ERROR_PrintLog(void);
void ERROR_SaveToEEPROM(void);
void ERROR_LoadFromEEPROM(void);
u8 ERROR_HasCriticalErrors(void);
u8 ERROR_GetErrorsByCategory(ErrorCategory_t category);
ES_t ERROR_HandleRecovery(u16 errorCode);

// Convenience macros for common error reporting
#define ERROR_REPORT_SENSOR(code, data, desc) \
    ERROR_Report(code, ERROR_SEVERITY_ERROR, ERROR_CATEGORY_SENSOR, data, desc)

#define ERROR_REPORT_COMM(code, data, desc) \
    ERROR_Report(code, ERROR_SEVERITY_WARNING, ERROR_CATEGORY_COMMUNICATION, data, desc)

#define ERROR_REPORT_CRITICAL(code, data, desc) \
    ERROR_Report(code, ERROR_SEVERITY_CRITICAL, ERROR_CATEGORY_SYSTEM, data, desc)

#endif /* ERROR_HANDLER_H_ */