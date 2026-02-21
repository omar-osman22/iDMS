#include "inc/app.h"
#include "Service/debug_log.h"
#include "Service/watchdog_timer.h"
#include "MCAL/Delay/delay.h"

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
    u32 systemTime = 0;
#endif

    // Enable watchdog monitoring after initialization
    WATCHDOG_Enable();

    while(1)
    {
#ifdef DEBUG_MODE
        loopCounter++;
        systemTime += 100; // Approximate 100ms per loop iteration
        
        // Update watchdog system time
        WATCHDOG_UpdateTime(systemTime);
        
        if (loopCounter % 10 == 0) {  // Log every 10 iterations to avoid flooding
            DEBUG_LogInfo("Main loop running");
            DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Loop iteration", loopCounter);
            DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "System time (ms)", systemTime);
        }
        
        // Print watchdog status periodically
        if (loopCounter % 100 == 0) {
            WATCHDOG_PrintStatus();
        }
#endif

        // Process user input and phone list operations
        APP_Start();

#ifdef DEBUG_MODE
        // Log system readings
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "Temperature (scaled)", (int)(APP_GetFilteredTemperature() * 100));
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "Current (scaled)", (int)(APP_GetFilteredCurrent() * 100));
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "Power (scaled)", (int)(APP_GetCalculatedPower() * 100));
        
        if (APP_IsSystemInAlarm()) {
            DEBUG_LogError("SYSTEM IN ALARM STATE");
        }
#endif

        // Delay for 1 second
        MCAL_DelayMs(1000);
    }
    
    return 0;
}