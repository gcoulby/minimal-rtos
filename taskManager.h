#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdio.h>
#include "pico/stdlib.h"

#define MAX_TASKS 5

typedef void (*TaskFunction_t)(void *);

enum TaskPriority
{
    HIGH_PRIORITY,
    MEDIUM_PRIORITY,
    LOW_PRIORITY
};

enum SchedulerType
{
    ROUND_ROBIN,
    PRIORITY_BASED
};

typedef struct
{
    TaskFunction_t taskFunction;
    void *parameters;
    uint32_t delay;
    uint32_t period;
    TaskPriority priority;
    uint8_t isActive;
} Task;

class TaskManager
{
public:
    TaskManager(SchedulerType schedulerType = ROUND_ROBIN);
    void createTask(TaskFunction_t function, void *params, uint32_t delay, uint32_t period, TaskPriority priority = LOW_PRIORITY);
    void scheduler();

private:
    Task taskList[MAX_TASKS];
    SchedulerType schedulerType;
    void roundRobinScheduler();
    void priorityBasedScheduler();
};

#endif // TASK_MANAGER_H