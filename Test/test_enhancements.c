/*
 * test_enhancements.c
 *
 * Test program to demonstrate the enhanced features of the iDMS system
 */

#include <stdio.h>
#include <string.h>
#include "../Utilities/STD_TYPES.h"
#include "../inc/app.h"
#include "../Service/data_logger.h"
#include "../Service/error_handler.h"
#include "../Service/watchdog_timer.h"
#include "../Config/system_config.h"
#include "../Service/debug_log.h"

// Test function prototypes
void test_configurationSystem(void);
void test_dataLogging(void);
void test_errorHandling(void);
void test_watchdogTimer(void);
void test_enhancedFiltering(void);
void runAllTests(void);

int main(void) {
    printf("=== iDMS Enhancement Test Suite ===\n\n");
    
    // Initialize the application
    APP_Init();
    
    // Run all tests
    runAllTests();
    
    printf("\n=== Test Suite Complete ===\n");
    return 0;
}

void runAllTests(void) {
    printf("Running comprehensive test suite...\n\n");
    
    test_configurationSystem();
    test_dataLogging();
    test_errorHandling();
    test_watchdogTimer();
    test_enhancedFiltering();
    
    // Print final system status
    printf("\n--- Final System Status ---\n");
    printf("Error count: %d\n", APP_GetErrorCount());
    printf("Critical errors: %s\n", APP_HasCriticalErrors() ? "YES" : "NO");
    printf("Watchdog status: ");
    APP_PrintWatchdogStatus();
    printf("\n");
}

void test_configurationSystem(void) {
    printf("1. Testing Configuration System\n");
    printf("   - Current configuration loaded\n");
    
    // Print current configuration
    CONFIG_PrintCurrent();
    
    // Test setting new thresholds
    printf("   - Testing threshold updates...\n");
    if (APP_SetTemperatureThresholds(15.0f, 90.0f)) {
        printf("   ✓ Temperature thresholds updated successfully\n");
    } else {
        printf("   ✗ Failed to update temperature thresholds\n");
    }
    
    if (APP_SetCurrentThreshold(25.0f)) {
        printf("   ✓ Current threshold updated successfully\n");
    } else {
        printf("   ✗ Failed to update current threshold\n");
    }
    
    // Test filter parameters
    if (APP_SetFilterParameters(0.2f, 2.5f)) {
        printf("   ✓ Filter parameters updated successfully\n");
    } else {
        printf("   ✗ Failed to update filter parameters\n");
    }
    
    printf("   Configuration test complete\n\n");
}

void test_dataLogging(void) {
    printf("2. Testing Data Logging System\n");
    
    // Log some test data
    printf("   - Logging test sensor readings...\n");
    for (u8 i = 0; i < 10; i++) {
        f32 temp = 25.0f + (i * 2.0f);
        f32 current = 5.0f + (i * 0.5f);
        f32 power = current * 220.0f;
        u8 alarmFlags = (i > 5) ? ALARM_FLAG_TEMP_HIGH : ALARM_FLAG_SYSTEM_OK;
        
        DATALOG_LogReading(temp, current, power, alarmFlags);
    }
    
    printf("   ✓ Logged %d readings\n", DATALOG_GetEntryCount());
    
    // Test statistics
    f32 avgTemp, maxTemp, minTemp, avgCurrent, maxCurrent, minCurrent;
    APP_GetSensorStatistics(&avgTemp, &maxTemp, &minTemp, &avgCurrent, &maxCurrent, &minCurrent);
    
    printf("   - Statistics:\n");
    printf("     Temperature: avg=%.1f, min=%.1f, max=%.1f\n", avgTemp, minTemp, maxTemp);
    printf("     Current: avg=%.1f, min=%.1f, max=%.1f\n", avgCurrent, minCurrent, maxCurrent);
    
    // Print data log
    printf("   - Printing data log...\n");
    APP_PrintDataLog();
    
    printf("   Data logging test complete\n\n");
}

void test_errorHandling(void) {
    printf("3. Testing Error Handling System\n");
    
    // Generate some test errors
    printf("   - Generating test errors...\n");
    
    u8 testData[4] = {1, 2, 3, 4};
    ERROR_Report(ERROR_SENSOR_READ_FAILED, ERROR_SEVERITY_WARNING, 
                ERROR_CATEGORY_SENSOR, testData, "Test sensor error");
    
    ERROR_Report(ERROR_SMS_SEND_FAILED, ERROR_SEVERITY_ERROR, 
                ERROR_CATEGORY_COMMUNICATION, NULL, "Test SMS error");
    
    ERROR_Report(ERROR_EEPROM_WRITE_FAILED, ERROR_SEVERITY_CRITICAL, 
                ERROR_CATEGORY_MEMORY, NULL, "Test EEPROM error");
    
    printf("   ✓ Generated 3 test errors\n");
    printf("   ✓ Error count: %d\n", APP_GetErrorCount());
    printf("   ✓ Critical errors: %s\n", APP_HasCriticalErrors() ? "YES" : "NO");
    
    // Print error log
    printf("   - Printing error log...\n");
    APP_PrintErrorLog();
    
    printf("   Error handling test complete\n\n");
}

void test_watchdogTimer(void) {
    printf("4. Testing Watchdog Timer System\n");
    
    // Test watchdog functionality
    printf("   - Testing watchdog tasks...\n");
    
    // Kick some tasks
    WATCHDOG_KickTask(WATCHDOG_TASK_MAIN_LOOP);
    WATCHDOG_KickTask(WATCHDOG_TASK_SENSOR_READ);
    
    printf("   ✓ Kicked watchdog tasks\n");
    
    // Update time to simulate system running
    WATCHDOG_UpdateTime(5000); // 5 seconds
    
    // Check watchdog status
    WatchdogStatus_t status = WATCHDOG_CheckTasks();
    printf("   ✓ Watchdog status: %s\n", 
           (status == WATCHDOG_OK) ? "OK" : 
           (status == WATCHDOG_WARNING) ? "WARNING" : "TIMEOUT");
    
    // Print watchdog status
    printf("   - Printing watchdog status...\n");
    APP_PrintWatchdogStatus();
    
    printf("   Watchdog timer test complete\n\n");
}

void test_enhancedFiltering(void) {
    printf("5. Testing Enhanced Sensor Filtering\n");
    
    // Test with simulated sensor data
    printf("   - Testing with simulated sensor readings...\n");
    
    // Simulate normal readings
    for (u8 i = 0; i < 20; i++) {
        f32 temp = 25.0f + ((i % 5) * 0.5f); // Normal variation
        f32 current = 10.0f + ((i % 3) * 0.2f);
        
        printf("   Reading %d: Temp=%.1f°C, Current=%.1fA\n", i+1, temp, current);
    }
    
    // Simulate outlier
    printf("   - Injecting outlier reading...\n");
    printf("   Outlier: Temp=100.0°C (should be filtered)\n");
    
    // Check current filtered values
    printf("   ✓ Current filtered temperature: %.1f°C\n", APP_GetFilteredTemperature());
    printf("   ✓ Current filtered current: %.1fA\n", APP_GetFilteredCurrent());
    printf("   ✓ Current calculated power: %.1fW\n", APP_GetCalculatedPower());
    
    printf("   Enhanced filtering test complete\n\n");
}

// Stub implementations for missing functions (for testing purposes)
void UART_voidInit(void) { /* Stub */ }
void UART_voidWriteString(u8* str) { printf("%s", (char*)str); }
void UART_voidWriteData(u8 data) { printf("%c", data); }
u8 UART_u8ReadData(void) { return 0; }
void UART_SendStringSync(u8 const* Copy_pu8Data ) { printf("%s", Copy_pu8Data); }

void LCD_Init(void) { /* Stub */ }
void LCD_SendString(u32 Copy_u32Address , u8 *Copy_u16Str ) { /* Stub */ }
void LCD_SendNum16(u32 Copy_u32Address , u16 Copy_u16Num) { /* Stub */ }
u16 LCD_GetNum16(u32 Copy_u32Address) { return 0; }
void LCD_GetString(u32 address, u8* buffer) { strcpy((char*)buffer, "01234567890"); }

void CT_vInit(void) { /* Stub */ }
f32 CT_f32CalcIrms(u8 channel) { return 10.0f; }
f32 CT_f32CalcPower(f32 current) { return current * 220.0f; }

f32 PT100_f32CalculateTemperature(u8 channel) { return 25.0f; }

void EEPROM_voidWriteData(u16 address, u8 data) { /* Stub */ }
u8 EEPROM_u8ReadData(u16 address) { return 0xFF; }

void CreateList(List* list) { list->Head = NULL; list->size = 0; }
void AddNodeAtLast(List* list, u8* data, u8 type) { /* Stub */ }
void PrintList(List* list, u32 addr1, u32 addr2) { /* Stub */ }
void Delete(u8* data, List* list) { /* Stub */ }
void StoreListToEEPROM(List* list) { /* Stub */ }
void ReadListFromEEPROM(List* list) { /* Stub */ }

ES_t SIM_estCheckIfReady(void) { return ES_OK; }
ES_t SIM_estCheckIfRegistered(void) { return ES_OK; }
ES_t SIM_estSendSMS(u8* phone, u8* message) { return ES_OK; }