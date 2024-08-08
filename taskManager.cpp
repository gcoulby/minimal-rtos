#include <taskManager.h> // Include the task manager header file

/**
 * @brief This initialises the TaskManager object
 * @param schedulerType The type of scheduler to use (ROUND_ROBIN or PRIORITY_BASED)
 * @note The default scheduler is ROUND_ROBIN
 * @note This constructor initialises all tasks as inactive
 * @note The taskList array is initialised with a size of MAX_TASKS (defined in taskManager.h)
 */
TaskManager::TaskManager(SchedulerType schedulerType)
{
    this->schedulerType = schedulerType;
    for (int i = 0; i < MAX_TASKS; i++)
    {
        taskList[i].isActive = 0;
    }
};

/**
 * @brief This function creates a task
 * @param function The function to run as a task
 * @param params The parameters to pass to the task
 * @param delay The delay before the task runs for the first time
 * @param period The period at which the task runs
 * @param priority The priority of the task (HIGH_PRIORITY, MEDIUM_PRIORITY, LOW_PRIORITY)
 * @note This function creates a task with the given parameters
 * @note The task is added to the taskList array
 * @note The task is added to the first available slot in the taskList array
 */
void TaskManager::createTask(TaskFunction_t function, void *params, uint32_t delay, uint32_t period, TaskPriority priority)
{
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (!taskList[i].isActive)
        {
            taskList[i].taskFunction = function;
            taskList[i].parameters = params;
            taskList[i].delay = delay;
            taskList[i].period = period;
            taskList[i].priority = priority;
            taskList[i].isActive = 1;
            break;
        }
    }
};

/**
 * @brief This function is the main scheduler function
 * @note This function calls the appropriate scheduler based on the schedulerType
 */
void TaskManager::scheduler()
{
    switch (schedulerType)
    {
    case ROUND_ROBIN:
        roundRobinScheduler();
        break;
    case PRIORITY_BASED:
        priorityBasedScheduler();
        break;
    default:
        break;
    }
};

/**
 * @brief This function is the scheduler that runs the tasks
 * @note This is a simple scheduler that runs the tasks in a round-robin fashion
 * @note The tasks are executed in the order they are created
 */
void TaskManager::roundRobinScheduler()
{
    while (1)
    {
        for (int i = 0; i < MAX_TASKS; i++)
        {
            if (taskList[i].isActive)
            {
                if (taskList[i].delay == 0)
                {
                    taskList[i].taskFunction(taskList[i].parameters);
                    taskList[i].delay = taskList[i].period;
                }
                else
                {
                    taskList[i].delay--;
                }
            }
        }
        sleep_ms(1); // Basic delay for simplicity
    }
};

/**
 * @brief This function is the scheduler that runs the tasks
 * @note This is a simple scheduler that runs the tasks based on their priority
 * @note The tasks with the highest priority are executed first
 * @note If two tasks have the same priority, tasks are executed in round-robin fashion
 */
void TaskManager::priorityBasedScheduler()
{
    while (1)
    {
        uint8_t highestPriority = 255;
        int highestPriorityTask = -1;

        for (int i = 0; i < MAX_TASKS; i++)
        {
            if (taskList[i].isActive && taskList[i].delay == 0 && taskList[i].priority < highestPriority)
            {
                highestPriority = taskList[i].priority;
                highestPriorityTask = i;
            }
        }

        if (highestPriorityTask != -1)
        {
            taskList[highestPriorityTask].taskFunction(taskList[highestPriorityTask].parameters);
            taskList[highestPriorityTask].delay = taskList[highestPriorityTask].period;
        }

        for (int i = 0; i < MAX_TASKS; i++)
        {
            if (taskList[i].isActive && taskList[i].delay > 0)
            {
                taskList[i].delay--;
            }
        }

        sleep_ms(1); // Basic delay for simplicity
    }
};
