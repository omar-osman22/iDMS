#include "inc/app.h"

int main(void)
{
    // Initialize application
    APP_Init();
    
    while(1)
    {
        // Process user input and phone list operations
        APP_Start();

        updateDisplayReadings(&appState);

        // Delay for 1 second
        for(u32 i = 0; i < 1000000; i++);
    }
    
    return 0;
}