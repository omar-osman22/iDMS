#ifndef APP_H_
#define APP_H_

#include "../Utilities/STD_TYPES.h"
#include "../Utilities/BIT_MATH.h"
#include "../Service/PhoneList.h"  // Changed from LinkedList.h to PhoneList.h
#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/ADC/ADC_interface.h"
#include "../MCAL/UART/UART_Interface.h"
#include "../Service/TopWayLCD_Interface.h"
#include "../Service/PT100_interface.h"
#include "../Service/CTcurrent_interface.h"

// Configuration Constants
#define VALID_PHONE_LENGTH      11
#define TEMPERATURE_OFFSET      50
#define MAX_TEMPERATURE         85.0f    // Maximum safe temperature in Celsius
#define MIN_TEMPERATURE         -20.0f   // Minimum safe temperature in Celsius
#define MAX_CURRENT            20.0f     // Maximum safe current in Amperes
#define ALARM_DELAY_MS         5000      // Delay between alarm messages
#define SMS_BUFFER_SIZE        64
#define LCD_LINES              4         // Number of lines on the LCD display

// Sensor Configuration
#define TEMP_SENSOR_CHANNEL    Channel_A0
#define CURRENT_SENSOR_CHANNEL Channel_A1
#define FILTER_WINDOW_SIZE     10  // Number of samples for moving average

// List Types (from LCD interface)
#define Calling_list     1
#define SMS_list         2

// Debug Configuration
#define DEBUG_MODE
#define TEST_ITERATIONS  100


// Sensor Reading Structure with Filtering
typedef struct {
    f32 raw;                    // Raw sensor reading
    f32 filtered;               // Filtered sensor reading
    f32 history[FILTER_WINDOW_SIZE]; // Historical readings for filtering
    u8 historyIndex;           // Current index in history array
    f32 sum;                   // Running sum for moving average
} SensorReading;

// Temperature and Power Readings Structure
typedef struct {
    SensorReading temperature;
    SensorReading current;
    f32 power;
    u32 lastAlarmTime;
    u8 alarmActive;
} SystemReadings;

// Application State Structure
typedef struct {
    List phoneList;         // Changed from LinkedList* to List (direct struct, not pointer)
    u8 phoneNumberSize;
    u8 isValidPhoneNumber;
    u8 inputData[SMS_BUFFER_SIZE];  // Changed to array for LCD compatibility
    u8 phoneNumber[SMS_BUFFER_SIZE]; // Changed to array for LCD compatibility
    SystemReadings readings;
    u32 currentTime;
} AppState;

// Global application state
static AppState appState = {0};

void updateDisplayReadings(const AppState* state);

// Function Prototypes
void APP_Init(void);
void APP_Start(void);
//void APP_UpdateDisplay(void);
u8 APP_HandlePhoneNumber(const char* phoneNumber);

// Utility Function Prototypes
f32 APP_GetFilteredTemperature(void);
f32 APP_GetFilteredCurrent(void);
f32 APP_GetCalculatedPower(void);
u8 APP_IsSystemInAlarm(void);

#endif /* APP_H_ */