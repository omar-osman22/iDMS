#include "../inc/app.h"
#include <math.h>
#include "../Debug/debug_log.h"
#include "../Service/enhanced_sms.h"
#include "../Service/data_logger.h"
#include "../Service/watchdog_timer.h"
#include "../Service/error_handler.h"

// Private function prototypes
static void initializeSystem(void);
static void clearLCDDisplay(const char* clear);
static u8 validatePhoneNumber(const char* phoneNumber, u8* phoneNumberSize);
static void handlePhoneListOperations(LinkedList* phoneList, const char* phoneNumber, u8 isValid);
static void updateSensorReadings(SystemReadings* readings);
static void updateSensorFilter(SensorReading* sensor, f32 newReading);
static void enhancedSensorFilter(SensorReading* sensor, f32 newReading);
static u8 isOutlier(SensorReading* sensor, f32 reading);
static void calibrateSensor(SensorReading* sensor);
static void sendAlarmMessage(const u8* phoneNumber, const u8* message);
static void printListToLCD(LinkedList* phoneList);
static void checkSafetyThresholds(AppState* state); // Added missing function prototype


void APP_Init(void)
{
    // Initialize error handling first
    ERROR_Init();
    
    // Initialize configuration system 
    CONFIG_Init();
    
    // Initialize watchdog timer
    WATCHDOG_Init();
    
    // Initialize enhanced SMS service
    SMS_Init();
    
    // Initialize data logger
    DATALOG_Init();
    
    // Initialize hardware
    initializeSystem();
    
    // Initialize application state
    CreateList(&appState.phoneList);
    appState.phoneNumberSize = 0;
    appState.isValidPhoneNumber = 0;
    appState.inputData[0] = '\0';
    appState.phoneNumber[0] = '\0';
    
    // Initialize sensor readings and enhanced filter buffers
    for(u8 i = 0; i < FILTER_WINDOW_SIZE; i++) {
        appState.readings.temperature.history[i] = 0;
        appState.readings.current.history[i] = 0;
    }
    appState.readings.temperature.historyIndex = 0;
    appState.readings.current.historyIndex = 0;
    appState.readings.temperature.sum = 0;
    appState.readings.current.sum = 0;
    
    // Initialize enhanced filtering parameters
    appState.readings.temperature.variance = 0;
    appState.readings.temperature.mean = 0;
    appState.readings.temperature.sampleCount = 0;
    appState.readings.temperature.isCalibrated = 0;
    
    appState.readings.current.variance = 0;
    appState.readings.current.mean = 0;
    appState.readings.current.sampleCount = 0;
    appState.readings.current.isCalibrated = 0;
    
    appState.readings.lastAlarmTime = 0;
    appState.readings.alarmActive = 0;
    
    // Initialize sensors
    CT_vInit();
    
    // Initialize display
    clearLCDDisplay((const u8*)" ");
    ReadListFromEEPROM(&appState.phoneList);
}

void APP_Start(void)
{
    // Kick main loop watchdog
    WATCHDOG_KickTask(WATCHDOG_TASK_MAIN_LOOP);
    
    // Reset phone number size for new iteration
    appState.phoneNumberSize = 0;

    // Get input from LCD
    LCD_GetString(Phone, appState.inputData);
    for(u8 i = 0; i < SMS_BUFFER_SIZE; i++) {
        appState.phoneNumber[i] = appState.inputData[i];
        if(appState.inputData[i] == '\0') break;
    }

    // Validate phone number
    appState.isValidPhoneNumber = validatePhoneNumber(appState.phoneNumber, &appState.phoneNumberSize);

    // Handle phone list operations
    WATCHDOG_KickTask(WATCHDOG_TASK_PHONE_MGMT);
    handlePhoneListOperations(&appState.phoneList, appState.phoneNumber, appState.isValidPhoneNumber);

    // Update and check sensor readings
    WATCHDOG_KickTask(WATCHDOG_TASK_SENSOR_READ);
    updateSensorReadings(&appState.readings);
    checkSafetyThresholds(&appState);

    // Update display
    WATCHDOG_KickTask(WATCHDOG_TASK_LCD_UPDATE);
    updateDisplayReadings(&appState);
    printListToLCD(&appState.phoneList);
    
    // Check watchdog status periodically
    static u8 watchdogCheckCounter = 0;
    watchdogCheckCounter++;
    if (watchdogCheckCounter >= 50) { // Check every 50 iterations
        watchdogCheckCounter = 0;
        WatchdogStatus_t status = WATCHDOG_CheckTasks();
        if (status != WATCHDOG_OK) {
            DEBUG_LogError("Watchdog check failed");
        }
    }
}

static void updateSensorReadings(SystemReadings* readings)
{
    // Read temperature with offset compensation and error handling
    f32 tempReading = PT100_f32CalculateTemperature(TEMP_SENSOR_CHANNEL);
    
    // Check for invalid temperature readings
    if (tempReading < -50.0f || tempReading > 200.0f) {
        u8 errorData[4] = {(u8)(tempReading), 0, 0, 0};
        ERROR_REPORT_SENSOR(ERROR_SENSOR_READ_FAILED, errorData, "Invalid temperature reading");
        tempReading = readings->temperature.filtered; // Use last valid reading
    }
    
    enhancedSensorFilter(&readings->temperature, tempReading);
    
    // Read current with error handling
    f32 currentReading = CT_f32CalcIrms(CURRENT_SENSOR_CHANNEL);
    
    // Check for invalid current readings
    if (currentReading < 0 || currentReading > 100.0f) {
        u8 errorData[4] = {(u8)(currentReading), 0, 0, 0};
        ERROR_REPORT_SENSOR(ERROR_SENSOR_READ_FAILED, errorData, "Invalid current reading");
        currentReading = readings->current.filtered; // Use last valid reading
    }
    
    enhancedSensorFilter(&readings->current, currentReading);
    
    // Calculate power with validation
    readings->power = CT_f32CalcPower(readings->current.filtered);
    if (readings->power < 0 || readings->power > 10000.0f) {
        u8 errorData[4] = {(u8)(readings->power), 0, 0, 0};
        ERROR_REPORT_SENSOR(ERROR_SENSOR_READ_FAILED, errorData, "Invalid power calculation");
        readings->power = 0; // Set to safe value
    }
    
    // Log readings for historical data (every 10th reading to reduce EEPROM wear)
    static u8 logCounter = 0;
    logCounter++;
    if (logCounter >= 10) {
        logCounter = 0;
        
        // Determine alarm flags
        u8 alarmFlags = ALARM_FLAG_SYSTEM_OK;
        if (readings->temperature.filtered > g_SystemConfig.tempThresholdMax) {
            alarmFlags |= ALARM_FLAG_TEMP_HIGH;
        }
        if (readings->temperature.filtered < g_SystemConfig.tempThresholdMin) {
            alarmFlags |= ALARM_FLAG_TEMP_LOW;
        }
        if (readings->current.filtered > g_SystemConfig.currentThresholdMax) {
            alarmFlags |= ALARM_FLAG_CURRENT_HIGH;
        }
        
        // Log the reading
        DATALOG_LogReading(readings->temperature.filtered, 
                          readings->current.filtered, 
                          readings->power, 
                          alarmFlags);
    }
}

static void updateSensorFilter(SensorReading* sensor, f32 newReading)
{
    // Legacy simple moving average filter (kept for backward compatibility)
    sensor->filtered = (sensor->filtered * (FILTER_WINDOW_SIZE - 1) + newReading) / FILTER_WINDOW_SIZE;
}

// Enhanced sensor filtering with outlier detection and IIR filter
static void enhancedSensorFilter(SensorReading* sensor, f32 newReading)
{
    sensor->raw = newReading;
    sensor->sampleCount++;
    
    // Initial calibration phase
    if (!sensor->isCalibrated && sensor->sampleCount <= g_SystemConfig.calibrationSamples) {
        calibrateSensor(sensor);
        return;
    }
    
    // Mark as calibrated after initial samples
    if (!sensor->isCalibrated && sensor->sampleCount > g_SystemConfig.calibrationSamples) {
        sensor->isCalibrated = 1;
        DEBUG_LogInfo("Sensor calibration completed");
    }
    
    // Check for outliers using statistical analysis
    if (sensor->isCalibrated && isOutlier(sensor, newReading)) {
        DEBUG_LogWarning("Outlier detected in sensor reading");
        u8 errorData[4] = {(u8)(newReading), (u8)(sensor->mean), 0, 0};
        ERROR_REPORT_SENSOR(ERROR_SENSOR_OUTLIER_DETECTED, errorData, "Sensor outlier detected");
        // Use previous filtered value for outliers
        newReading = sensor->filtered;
    }
    
    // Apply IIR (Infinite Impulse Response) filter for better noise reduction
    // Formula: y[n] = α * x[n] + (1-α) * y[n-1]
    if (sensor->sampleCount == 1) {
        // Initialize filter with first reading
        sensor->filtered = newReading;
    } else {
        sensor->filtered = g_SystemConfig.filterAlpha * newReading + (1.0f - g_SystemConfig.filterAlpha) * sensor->filtered;
    }
    
    // Update moving average backup (for comparison/fallback)
    sensor->sum -= sensor->history[sensor->historyIndex];
    sensor->history[sensor->historyIndex] = newReading;
    sensor->sum += newReading;
    sensor->historyIndex = (sensor->historyIndex + 1) % FILTER_WINDOW_SIZE;
    
    // Update running statistics for outlier detection
    f32 delta = newReading - sensor->mean;
    sensor->mean += delta / (f32)sensor->sampleCount;
    f32 delta2 = newReading - sensor->mean;
    sensor->variance += delta * delta2;
}

// Detect outliers using standard deviation
static u8 isOutlier(SensorReading* sensor, f32 reading)
{
    if (sensor->sampleCount < 10) return 0; // Need minimum samples for statistics
    
    f32 stdDev = sqrt(sensor->variance / (f32)(sensor->sampleCount - 1));
    f32 deviation = fabs(reading - sensor->mean);
    
    return (deviation > g_SystemConfig.outlierThreshold * stdDev);
}

// Calibrate sensor during initial startup
static void calibrateSensor(SensorReading* sensor)
{
    // During calibration, use simple averaging
    if (sensor->sampleCount == 1) {
        sensor->filtered = sensor->raw;
        sensor->mean = sensor->raw;
        sensor->variance = 0;
    } else {
        // Update running average during calibration
        f32 delta = sensor->raw - sensor->mean;
        sensor->mean += delta / (f32)sensor->sampleCount;
        f32 delta2 = sensor->raw - sensor->mean;
        sensor->variance += delta * delta2;
        
        // Use simple average during calibration
        sensor->filtered = sensor->mean;
    }
    
    // Store in history for moving average backup
    sensor->history[sensor->historyIndex] = sensor->raw;
    sensor->sum += sensor->raw;
    sensor->historyIndex = (sensor->historyIndex + 1) % FILTER_WINDOW_SIZE;
}

void updateDisplayReadings(const AppState* state)
{
    // Update temperature displays with offset
    s16 displayTemp = (s16)(state->readings.temperature.filtered + TEMPERATURE_OFFSET);
    LCD_SendNum16(In, displayTemp);
    LCD_SendNum16(In_Num, (s16)state->readings.temperature.filtered);
    
    // Update current displays
    LCD_SendNum16(Out, (s16)(state->readings.current.filtered * 100)); // Scaled for display
    LCD_SendNum16(Out_Num, (s16)state->readings.current.filtered);

    // Clear input buffer if needed
    if(LCD_GetNum16(Clear_BUFF)) {
        LCD_SendString(Phone, (u8*)"");
        LCD_SendNum16(Clear_BUFF, 0);
    }
}

static void checkSafetyThresholds(AppState* state)
{
    static u32 lastAlarmTime = 0; // Static variable to keep track of the last alarm time
    u8 alarmMessage[SMS_BUFFER_SIZE];
    u8 alarmCondition = 0;
    
    // Check temperature thresholds using configurable values
    if (state->readings.temperature.filtered > g_SystemConfig.tempThresholdMax || 
        state->readings.temperature.filtered < g_SystemConfig.tempThresholdMin) {
        alarmCondition = 1;
        
        // Use enhanced SMS service for temperature alarms
        Node* current = state->phoneList.Head;
        while (current != NULL) {
            WATCHDOG_KickTask(WATCHDOG_TASK_SMS_SERVICE);
            ES_t result = SMS_SendTemperatureAlarm(current->value,
                                                  state->readings.temperature.filtered,
                                                  g_SystemConfig.tempThresholdMin,
                                                  g_SystemConfig.tempThresholdMax);
            if (result != ES_OK) {
                u8 errorData[4] = {0, 0, 0, 0};
                ERROR_REPORT_COMM(ERROR_SMS_SEND_FAILED, errorData, "Temperature alarm SMS failed");
            }
            current = current->Next;
        }
    }
    
    // Check current thresholds using configurable values
    if (state->readings.current.filtered > g_SystemConfig.currentThresholdMax) {
        alarmCondition = 1;
        
        // Use enhanced SMS service for current alarms
        Node* current = state->phoneList.Head;
        while (current != NULL) {
            WATCHDOG_KickTask(WATCHDOG_TASK_SMS_SERVICE);
            ES_t result = SMS_SendCurrentAlarm(current->value,
                                              state->readings.current.filtered,
                                              state->readings.power,
                                              g_SystemConfig.currentThresholdMax);
            if (result != ES_OK) {
                u8 errorData[4] = {0, 0, 0, 0};
                ERROR_REPORT_COMM(ERROR_SMS_SEND_FAILED, errorData, "Current alarm SMS failed");
            }
            current = current->Next;
        }
    }
    
    // Handle alarm state and messaging
    if (alarmCondition) {
        state->readings.alarmActive = 1;
        LCD_SendString(Validation, (u8*)"ALARM!");
        
        // Update alarm timing
        if (state->readings.lastAlarmTime == 0) {
            state->readings.lastAlarmTime = state->currentTime;
        }
    } else {
        state->readings.alarmActive = 0;
        LCD_SendString(Validation, (u8*)"System OK");
    }
}

u8 APP_HandlePhoneNumber(const char* phoneNumber)
{
    u8 isValid = validatePhoneNumber(phoneNumber, &appState.phoneNumberSize);
    if(isValid)
    {
        handlePhoneListOperations(&appState.phoneList, phoneNumber, isValid);
    }
    return isValid;
}

static void initializeSystem(void)
{
    UART_Init();
    LCD_Init();
    ADC_Init();
}

static u8 validatePhoneNumber(const char* phoneNumber, u8* phoneNumberSize)
{
    const char* validMessage = "Valid Number";
    const char* invalidMessage = "Invalid Number";
    
    if(phoneNumber[0] != '0' || phoneNumber[1] != '1')
    {
        LCD_SendString(Validation, invalidMessage);
        LCD_SendNum16(Add, 0);
        return 0;
    }

    *phoneNumberSize = 0;
    while(phoneNumber[*phoneNumberSize] != '\0')
    {
        (*phoneNumberSize)++;
    }

    if(*phoneNumberSize == VALID_PHONE_LENGTH)
    {
        LCD_SendString(Validation, validMessage);
        return 1;
    }

    LCD_SendString(Validation, invalidMessage);
    LCD_SendNum16(Add, 0);
    return 0;
}

static void handlePhoneListOperations(LinkedList* phoneList, const char* phoneNumber, u8 isValid)
{
    if(!isValid) return;

    if(LCD_GetNum16(Add))
    {
        u8 list = LCD_GetNum16(L_type);
        if(list == Calling_list || list == SMS_list)
        {
            LCD_SendString(Phone, phoneNumber);
            AddNodeAtLast(phoneList, phoneNumber, list);
            StoreListToEEPROM(phoneList);
        }
        LCD_SendNum16(Add, 0);
        LCD_SendNum16(Clear_BUFF, 1);
    }
    else if(LCD_GetNum16(Remove))
    {
        Delete(phoneNumber, phoneList);
        StoreListToEEPROM(phoneList);
        LCD_SendNum16(Remove, 0);
        LCD_SendNum16(Clear_BUFF, 1);
        clearLCDDisplay(" ");
    }
}

static void sendAlarmMessage(const u8* phoneNumber, const u8* message)
{
    UART_SendStringSync("AT+CMGS=\"");
    while(*phoneNumber) UART_voidWriteData(*phoneNumber++);
    UART_SendStringSync("\"\r\n");
    while(*message) UART_voidWriteData(*message++);
    UART_voidWriteData(0x1A); // Send Ctrl+Z to end message
}

static void printListToLCD(LinkedList* phoneList)
{
    PrintList(phoneList, CList_address, SList_address);
}

static void clearLCDDisplay(const char* clear)
{
    for(u8 i = 0; i < (LCD_LINES * 2); i++)
    {
        LCD_SendString(CList_address + (128 * i), clear);
    }
}

// Utility Function Implementations
f32 APP_GetFilteredTemperature(void) {
    return appState.readings.temperature.filtered;
}

f32 APP_GetFilteredCurrent(void) {
    return appState.readings.current.filtered;
}

f32 APP_GetCalculatedPower(void) {
    return appState.readings.power;
}

u8 APP_IsSystemInAlarm(void) {
    return appState.readings.alarmActive;
}

// Enhanced Feature Implementations
void APP_PrintDataLog(void) {
    DATALOG_PrintLog();
}

void APP_ClearDataLog(void) {
    DATALOG_ClearLog();
}

void APP_SendSystemStatus(const u8* phoneNumber) {
    if (SMS_ValidatePhoneNumber(phoneNumber)) {
        SMS_SendSystemStatus(phoneNumber,
                           appState.readings.temperature.filtered,
                           appState.readings.current.filtered,
                           appState.readings.power);
    }
}

void APP_GetSensorStatistics(f32* avgTemp, f32* maxTemp, f32* minTemp, 
                            f32* avgCurrent, f32* maxCurrent, f32* minCurrent) {
    DATALOG_GetStatistics(avgTemp, maxTemp, minTemp, avgCurrent, maxCurrent, minCurrent);
}

u8 APP_SetTemperatureThresholds(f32 min, f32 max) {
    return CONFIG_SetTempThresholds(min, max);
}

u8 APP_SetCurrentThreshold(f32 max) {
    return CONFIG_SetCurrentThreshold(max);
}

u8 APP_SetFilterParameters(f32 alpha, f32 outlierThreshold) {
    return CONFIG_SetFilterParameters(alpha, outlierThreshold);
}

// Error handling utility functions
void APP_PrintErrorLog(void) {
    ERROR_PrintLog();
}

void APP_ClearErrors(void) {
    ERROR_ClearAll();
}

u8 APP_GetErrorCount(void) {
    return ERROR_GetCount();
}

u8 APP_HasCriticalErrors(void) {
    return ERROR_HasCriticalErrors();
}

void APP_PrintWatchdogStatus(void) {
    WATCHDOG_PrintStatus();
}
