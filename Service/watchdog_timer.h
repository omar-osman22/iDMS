/*
 * watchdog_timer.h
 *
 * Software watchdog timer implementation for system reliability
 */

#ifndef WATCHDOG_TIMER_H_
#define WATCHDOG_TIMER_H_

#include "../Utilities/STD_TYPES.h"

// Watchdog status
typedef enum {
    WATCHDOG_OK,
    WATCHDOG_WARNING,
    WATCHDOG_TIMEOUT
} WatchdogStatus_t;

// Watchdog task structure
typedef struct {
    u8 taskId;              // Unique task identifier
    u32 lastKickTime;       // Last time task was kicked
    u32 timeoutMs;          // Timeout period in milliseconds
    u8 isActive;            // Task is active
    const char* taskName;   // Task name for debugging
} WatchdogTask_t;

// Maximum number of watchdog tasks
#define MAX_WATCHDOG_TASKS 8

// Predefined task IDs
#define WATCHDOG_TASK_MAIN_LOOP      0
#define WATCHDOG_TASK_SENSOR_READ    1
#define WATCHDOG_TASK_SMS_SERVICE    2
#define WATCHDOG_TASK_LCD_UPDATE     3
#define WATCHDOG_TASK_PHONE_MGMT     4

// Global watchdog state
typedef struct {
    WatchdogTask_t tasks[MAX_WATCHDOG_TASKS];
    u8 taskCount;
    u32 systemTime;
    u8 watchdogEnabled;
    u32 lastSystemReset;
} WatchdogTimer_t;

extern WatchdogTimer_t g_Watchdog;

// Function prototypes
void WATCHDOG_Init(void);
void WATCHDOG_Enable(void);
void WATCHDOG_Disable(void);
u8 WATCHDOG_RegisterTask(u8 taskId, const char* taskName, u32 timeoutMs);
void WATCHDOG_KickTask(u8 taskId);
void WATCHDOG_UpdateTime(u32 newTime);
WatchdogStatus_t WATCHDOG_CheckTasks(void);
void WATCHDOG_HandleTimeout(u8 taskId);
void WATCHDOG_PrintStatus(void);
u32 WATCHDOG_GetSystemUptime(void);

#endif /* WATCHDOG_TIMER_H_ */