#include "../inc/app.h"
#include "../Service/debug_log.h"
#include <stdio.h>

// Global application state
AppState appState = {0};

// Private function prototypes
static void initializeSystem(void);
static void clearLCDDisplay(const char* clear);
static u8 validatePhoneNumber(const char* phoneNumber, u8* phoneNumberSize);
static void handlePhoneListOperations(List* phoneList, const char* phoneNumber, u8 isValid);
static void updateSensorReadings(SystemReadings* readings);
static void updateSensorFilter(SensorReading* sensor, f32 newReading);
static void sendAlarmMessage(const u8* phoneNumber, const u8* message);
static void printListToLCD(List* phoneList);
static void checkSafetyThresholds(AppState* state); // Added missing function prototype

void APP_Init(void)
{
    // Initialize hardware
    initializeSystem();
    
    // Initialize application state
    CreateList(&appState.phoneList);
    appState.phoneNumberSize = 0;
    appState.isValidPhoneNumber = 0;
    appState.inputData[0] = '\0';
    appState.phoneNumber[0] = '\0';
    
    // Initialize sensor readings and filter buffers
    appState.readings.temperature.prevFiltered = 0;
    appState.readings.current.prevFiltered = 0;
    appState.readings.temperature.minValue = 1000.0f;
    appState.readings.temperature.maxValue = -1000.0f;
    appState.readings.current.minValue = 1000.0f;
    appState.readings.current.maxValue = -1000.0f;
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
    handlePhoneListOperations(&appState.phoneList, appState.phoneNumber, appState.isValidPhoneNumber);

    // Update and check sensor readings
    updateSensorReadings(&appState.readings);
    checkSafetyThresholds(&appState);

    // Update display
    updateDisplayReadings(&appState);
    printListToLCD(&appState.phoneList);
}

static void updateSensorReadings(SystemReadings* readings)
{
    // Read temperature with offset compensation
    f32 tempReading = PT100_f32CalculateTemperature(TEMP_SENSOR_CHANNEL);
    updateSensorFilter(&readings->temperature, tempReading);
    
    // Read current
    f32 currentReading = CT_f32CalcIrms(CURRENT_SENSOR_CHANNEL);
    updateSensorFilter(&readings->current, currentReading);
    
    // Calculate power
    readings->power = CT_f32CalcPower(readings->current.filteredValue);
}

static void updateSensorFilter(SensorReading* sensor, f32 newReading)
{
    // Apply filter to new reading
    sensor->prevFiltered = sensor->filteredValue;
    sensor->filteredValue = (sensor->filteredValue * (FILTER_WINDOW_SIZE - 1) + newReading) / FILTER_WINDOW_SIZE;
    
    // Update min/max
    if (sensor->filteredValue < sensor->minValue) sensor->minValue = sensor->filteredValue;
    if (sensor->filteredValue > sensor->maxValue) sensor->maxValue = sensor->filteredValue;
}

void updateDisplayReadings(const AppState* state)
{
    // Update temperature displays with offset
    s16 displayTemp = (s16)(state->readings.temperature.filteredValue + TEMPERATURE_OFFSET);
    LCD_SendNum16(In, displayTemp);
    LCD_SendNum16(In_Num, (s16)state->readings.temperature.filteredValue);
    
    // Update current displays
    LCD_SendNum16(Out, (s16)(state->readings.current.filteredValue * 100)); // Scaled for display
    LCD_SendNum16(Out_Num, (s16)state->readings.current.filteredValue);

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
    
    // Check temperature thresholds using filtered value
    if (state->readings.temperature.filteredValue > MAX_TEMPERATURE || 
        state->readings.temperature.filteredValue < MIN_TEMPERATURE) {
        alarmCondition = 1;
        snprintf((char*)alarmMessage, SMS_BUFFER_SIZE, 
                "TEMP ALARM: T=%.1f C",
                state->readings.temperature.filteredValue);
    }
    
    // Check current thresholds using filtered value
    if (state->readings.current.filteredValue > MAX_CURRENT) {
        alarmCondition = 1;
        snprintf((char*)alarmMessage, SMS_BUFFER_SIZE,
                "CURRENT ALARM: I=%.1fA P=%.1fW",
                state->readings.current.filteredValue,
                state->readings.power);
    }
    
    // Handle alarm state and messaging
    if (alarmCondition) {
        state->readings.alarmActive = 1;
        LCD_SendString(Validation, (u8*)"ALARM!");
        
        // Send alarm message if enough time has passed
        if (state->readings.lastAlarmTime == 0 || state->readings.lastAlarmTime + ALARM_DELAY_MS <= state->currentTime) {
            Node* current = state->phoneList.Head;  // Changed from ->head to .Head
            while (current != NULL) {
                sendAlarmMessage((const u8*)current->value, alarmMessage);  // Changed from ->data to ->value
                current = current->Next;  // Changed from ->next to ->Next
            }
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

u8 APP_IsSystemInAlarm(void)
{
    // Return the current alarm state from the application state
    return appState.readings.alarmActive;
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
    
    DEBUG_LogInfo("Validating phone number");
    DEBUG_LogMessageWithValue(LOG_LEVEL_DEBUG, "Phone number: ", phoneNumber);
    
    if(phoneNumber[0] != '0' || phoneNumber[1] != '1')
    {
        DEBUG_LogError("Invalid phone number format (must start with 01)");
        LCD_SendString(Validation, invalidMessage);
        LCD_SendNum16(Add, 0);
        return 0;
    }

    *phoneNumberSize = 0;
    while(phoneNumber[*phoneNumberSize] != '\0')
    {
        (*phoneNumberSize)++;
    }
    
    DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Phone number length: ", *phoneNumberSize);

    if(*phoneNumberSize == VALID_PHONE_LENGTH)
    {
        DEBUG_LogInfo("Phone number validation successful");
        LCD_SendString(Validation, validMessage);
        return 1;
    }

    DEBUG_LogError("Invalid phone number length");
    DEBUG_LogValueInt(LOG_LEVEL_ERROR, "Expected length: ", VALID_PHONE_LENGTH);
    DEBUG_LogValueInt(LOG_LEVEL_ERROR, "Actual length: ", *phoneNumberSize);
    LCD_SendString(Validation, invalidMessage);
    LCD_SendNum16(Add, 0);
    return 0;
}

static void handlePhoneListOperations(List* phoneList, const char* phoneNumber, u8 isValid)
{
    if(!isValid) {
        DEBUG_LogWarning("Attempted operation with invalid phone number");
        return;
    }

    if(LCD_GetNum16(Add))
    {
        DEBUG_LogInfo("Add button pressed");
        u8 list = LCD_GetNum16(L_type);
        DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "List type: ", list);
        
        if(list == Calling_list || list == SMS_list)
        {
            DEBUG_LogInfo("Adding phone number to list");
            LCD_SendString(Phone, phoneNumber);
            AddNodeAtLast(phoneList, phoneNumber, list);
            DEBUG_LogInfo("Storing list to EEPROM");
            StoreListToEEPROM(phoneList);
            DEBUG_LogInfo("EEPROM storage complete");
        } else {
            DEBUG_LogWarning("Invalid list type selected");
        }
        LCD_SendNum16(Add, 0);
        LCD_SendNum16(Clear_BUFF, 1);
    }
    else if(LCD_GetNum16(Remove))
    {
        DEBUG_LogInfo("Remove button pressed");
        DEBUG_LogMessageWithValue(LOG_LEVEL_DEBUG, "Removing number: ", phoneNumber);
        Delete(phoneNumber, phoneList);
        DEBUG_LogInfo("Updating EEPROM after removal");
        StoreListToEEPROM(phoneList);
        LCD_SendNum16(Remove, 0);
        LCD_SendNum16(Clear_BUFF, 1);
        clearLCDDisplay(" ");
        DEBUG_LogInfo("Phone number removed successfully");
    } else {
        DEBUG_LogDebug("No add or remove action requested");
    }
}

static void sendAlarmMessage(const u8* phoneNumber, const u8* message)
{
    UART_SendStringSync((const u8*)"AT+CMGS=\"");
    while(*phoneNumber) UART_SendByteSynch(*phoneNumber++);
    UART_SendStringSync((const u8*)"\"\r\n");
    while(*message) UART_SendByteSynch(*message++);
    UART_SendByteSynch(0x1A); // Send Ctrl+Z to end message
}

static void printListToLCD(List* phoneList)
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

// Utility Functions
f32 APP_GetFilteredTemperature(void) {
    return appState.readings.temperature.filteredValue;
}

f32 APP_GetFilteredCurrent(void) {
    return appState.readings.current.filteredValue;
}

f32 APP_GetCalculatedPower(void) {
    return appState.readings.power;
}
