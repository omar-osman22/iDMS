/*
 * watchdog_timer.c
 *
 * Implementation of software watchdog timer for system reliability
 */

#include "watchdog_timer.h"
#include "../Service/debug_log.h"
#include "../Config/system_config.h"
#include <string.h>

// Global watchdog instance
WatchdogTimer_t g_Watchdog;

// Initialize watchdog timer
void WATCHDOG_Init(void) {
    DEBUG_LogInfo("Initializing watchdog timer");
    
    // Initialize watchdog state
    memset(&g_Watchdog, 0, sizeof(WatchdogTimer_t));
    g_Watchdog.taskCount = 0;
    g_Watchdog.systemTime = 0;
    g_Watchdog.watchdogEnabled = 0;
    g_Watchdog.lastSystemReset = 0;
    
    // Register default system tasks
    WATCHDOG_RegisterTask(WATCHDOG_TASK_MAIN_LOOP, "Main Loop", 10000);      // 10 seconds
    WATCHDOG_RegisterTask(WATCHDOG_TASK_SENSOR_READ, "Sensor Read", 5000);   // 5 seconds
    WATCHDOG_RegisterTask(WATCHDOG_TASK_SMS_SERVICE, "SMS Service", 30000);  // 30 seconds
    WATCHDOG_RegisterTask(WATCHDOG_TASK_LCD_UPDATE, "LCD Update", 8000);     // 8 seconds
    WATCHDOG_RegisterTask(WATCHDOG_TASK_PHONE_MGMT, "Phone Mgmt", 15000);    // 15 seconds
    
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Registered tasks", g_Watchdog.taskCount);
    DEBUG_LogInfo("Watchdog timer initialized");
}

// Enable watchdog monitoring
void WATCHDOG_Enable(void) {
    g_Watchdog.watchdogEnabled = 1;
    DEBUG_LogInfo("Watchdog monitoring enabled");
}

// Disable watchdog monitoring
void WATCHDOG_Disable(void) {
    g_Watchdog.watchdogEnabled = 0;
    DEBUG_LogInfo("Watchdog monitoring disabled");
}

// Register a new watchdog task
u8 WATCHDOG_RegisterTask(u8 taskId, const char* taskName, u32 timeoutMs) {
    if (g_Watchdog.taskCount >= MAX_WATCHDOG_TASKS) {
        DEBUG_LogError("Maximum watchdog tasks reached");
        return 0;
    }
    
    // Check if task ID already exists
    for (u8 i = 0; i < g_Watchdog.taskCount; i++) {
        if (g_Watchdog.tasks[i].taskId == taskId) {
            DEBUG_LogError("Task ID already exists");
            return 0;
        }
    }
    
    // Register new task
    WatchdogTask_t* task = &g_Watchdog.tasks[g_Watchdog.taskCount];
    task->taskId = taskId;
    task->lastKickTime = g_Watchdog.systemTime;
    task->timeoutMs = timeoutMs;
    task->isActive = 1;
    task->taskName = taskName;
    
    g_Watchdog.taskCount++;
    
    DEBUG_LogMessageWithValue(LOG_LEVEL_INFO, "Registered watchdog task", taskName);
    DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Task timeout (ms)", timeoutMs);
    
    return 1;
}

// Kick (reset) a watchdog task
void WATCHDOG_KickTask(u8 taskId) {
    for (u8 i = 0; i < g_Watchdog.taskCount; i++) {
        if (g_Watchdog.tasks[i].taskId == taskId && g_Watchdog.tasks[i].isActive) {
            g_Watchdog.tasks[i].lastKickTime = g_Watchdog.systemTime;
            DEBUG_LogValueInt(LOG_LEVEL_DEBUG, "Kicked task", taskId);
            return;
        }
    }
    
    DEBUG_LogValueInt(LOG_LEVEL_WARNING, "Unknown task kick attempt", taskId);
}

// Update system time (should be called regularly)
void WATCHDOG_UpdateTime(u32 newTime) {
    g_Watchdog.systemTime = newTime;
}

// Check all watchdog tasks for timeouts
WatchdogStatus_t WATCHDOG_CheckTasks(void) {
    if (!g_Watchdog.watchdogEnabled) {
        return WATCHDOG_OK;
    }
    
    WatchdogStatus_t overallStatus = WATCHDOG_OK;
    
    for (u8 i = 0; i < g_Watchdog.taskCount; i++) {
        WatchdogTask_t* task = &g_Watchdog.tasks[i];
        
        if (!task->isActive) continue;
        
        u32 timeSinceKick = g_Watchdog.systemTime - task->lastKickTime;
        
        if (timeSinceKick > task->timeoutMs) {
            // Task has timed out
            DEBUG_LogError("Watchdog timeout detected");
            DEBUG_LogMessageWithValue(LOG_LEVEL_ERROR, "Timed out task", task->taskName);
            DEBUG_LogValueInt(LOG_LEVEL_ERROR, "Time since kick (ms)", timeSinceKick);
            
            WATCHDOG_HandleTimeout(task->taskId);
            overallStatus = WATCHDOG_TIMEOUT;
        } else if (timeSinceKick > (task->timeoutMs * 3 / 4)) {
            // Task approaching timeout (warning)
            DEBUG_LogWarning("Watchdog warning - task approaching timeout");
            DEBUG_LogMessageWithValue(LOG_LEVEL_WARNING, "Warning task", task->taskName);
            
            if (overallStatus == WATCHDOG_OK) {
                overallStatus = WATCHDOG_WARNING;
            }
        }
    }
    
    return overallStatus;
}

// Handle watchdog timeout
void WATCHDOG_HandleTimeout(u8 taskId) {
    DEBUG_LogValueInt(LOG_LEVEL_ERROR, "Handling timeout for task", taskId);
    
    // Log timeout event
    DEBUG_LogError("SYSTEM WATCHDOG TIMEOUT DETECTED");
    
    // In a real embedded system, this might trigger:
    // 1. System reset
    // 2. Safe mode activation
    // 3. Emergency shutdown
    // 4. Alert notifications
    
    // For now, we'll log the event and try to recover
    switch (taskId) {
        case WATCHDOG_TASK_MAIN_LOOP:
            DEBUG_LogError("Main loop timeout - system may be hung");
            // In real system: trigger immediate reset
            break;
            
        case WATCHDOG_TASK_SENSOR_READ:
            DEBUG_LogError("Sensor reading timeout - check hardware");
            // Could try to reinitialize sensors
            break;
            
        case WATCHDOG_TASK_SMS_SERVICE:
            DEBUG_LogError("SMS service timeout - communication issue");
            // Could try to reinitialize SIM module
            break;
            
        case WATCHDOG_TASK_LCD_UPDATE:
            DEBUG_LogError("LCD update timeout - display issue");
            // Could try to reinitialize LCD
            break;
            
        case WATCHDOG_TASK_PHONE_MGMT:
            DEBUG_LogError("Phone management timeout");
            // Could reset phone list operations
            break;
            
        default:
            DEBUG_LogError("Unknown task timeout");
            break;
    }
    
    // Mark the task as needing attention
    for (u8 i = 0; i < g_Watchdog.taskCount; i++) {
        if (g_Watchdog.tasks[i].taskId == taskId) {
            // Reset the kick time to give task a chance to recover
            g_Watchdog.tasks[i].lastKickTime = g_Watchdog.systemTime;
            break;
        }
    }
}

// Print watchdog status for debugging
void WATCHDOG_PrintStatus(void) {
    DEBUG_LogInfo("=== Watchdog Status ===");
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Enabled", g_Watchdog.watchdogEnabled);
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "System time", g_Watchdog.systemTime);
    DEBUG_LogValueInt(LOG_LEVEL_INFO, "Task count", g_Watchdog.taskCount);
    
    for (u8 i = 0; i < g_Watchdog.taskCount; i++) {
        WatchdogTask_t* task = &g_Watchdog.tasks[i];
        u32 timeSinceKick = g_Watchdog.systemTime - task->lastKickTime;
        
        DEBUG_LogMessageWithValue(LOG_LEVEL_INFO, "Task", task->taskName);
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "  ID", task->taskId);
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "  Active", task->isActive);
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "  Timeout (ms)", task->timeoutMs);
        DEBUG_LogValueInt(LOG_LEVEL_INFO, "  Since kick (ms)", timeSinceKick);
    }
    
    DEBUG_LogInfo("=======================");
}

// Get system uptime in milliseconds
u32 WATCHDOG_GetSystemUptime(void) {
    return g_Watchdog.systemTime;
}