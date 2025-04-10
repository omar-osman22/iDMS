#include "inc/app.h"
#include "../Utilities/Debug/debug_log.h"

int main(void)
{
    // Initialize debug logging system
#ifdef DEBUG_MODE
    DEBUG_LogInit(LOG_LEVEL_DEBUG);
    DEBUG_LogInfo("System starting up");
#endif

    // Initialize application
    APP_Init();
    
#ifdef DEBUG_MODE
    DEBUG_LogInfo("Application initialized successfully");
    u32 loopCounter = 0;
#endif

    while(1)
    {
#ifdef DEBUG_MODE
        loopCounter++;
        if (loopCounter % 10 == 0) {  // Log every 10 iterations to avoid flooding
            DEBUG_LogInfo("Main loop running");
            DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Loop iteration", loopCounter);
        }
#endif

        // Process user input and phone list operations
        APP_Start();

#ifdef DEBUG_MODE
        // Log system readings
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "Temperature (scaled)", (int)(appState.readings.temperature.filtered * 100));
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "Current (scaled)", (int)(appState.readings.current.filtered * 100));
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "Power (scaled)", (int)(appState.readings.power * 100));
        
        if (APP_IsSystemInAlarm()) {
            DEBUG_LogError("SYSTEM IN ALARM STATE");
        }
#endif

        updateDisplayReadings(&appState);

        // Delay for 1 second
        for(u32 i = 0; i < 1000000; i++);
    }
    
    return 0;
}