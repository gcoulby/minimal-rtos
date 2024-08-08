/**
 * @file rtos.c
 * @brief A minimal RTOS using a round-robin scheduler for Pico
 * @author Graham Coulby
 * @date 2024-08-08
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h" // Include this header for ADC functions

#define MAX_TASKS 5

typedef void (*TaskFunction_t)(void *);

#define YELLOW_LED_PIN 18
#define RED_LED_PIN 19
#define BTN_PIN 20
#define POT_PIN 26

static bool led1_state = false;
static bool led2_state = false;
static int pot_value = 0;

typedef struct
{
    TaskFunction_t taskFunction;
    void *parameters;
    uint32_t delay;
    uint32_t period;
    uint8_t isActive;
} Task;

Task taskList[MAX_TASKS];

void createTask(TaskFunction_t function, void *params, uint32_t delay, uint32_t period)
{
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (!taskList[i].isActive)
        {
            taskList[i].taskFunction = function;
            taskList[i].parameters = params;
            taskList[i].delay = delay;
            taskList[i].period = period;
            taskList[i].isActive = 1;
            break;
        }
    }
}

/**
 * @brief This function is the scheduler that runs the tasks
 * @note This is a simple scheduler that runs the tasks in a round-robin fashion
 * @note The tasks are executed in the order they are created
 */
void scheduler()
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
}

void toggleYellowLED(void *params)
{
    gpio_put(YELLOW_LED_PIN, led1_state);
    led1_state = !led1_state;
}

void toggleRedLED(void *params)
{
    gpio_put(RED_LED_PIN, led2_state);
    led2_state = !led2_state;
}

void printValue(void *params)
{
    float voltage = pot_value * 3.3f / (1 << 12);
    printf("Potentiometer value: %d\nVoltage: %.2f\n", pot_value, voltage);
}

void readPotentiometerIfButtonPressed(void *params)
{
    // see if the button is pressed
    if (!gpio_get(BTN_PIN))
        return;

    adc_select_input(0);
    pot_value = adc_read();
}

/**
 * @brief This function initialises the GPIO pins
 * @note This initialises the yellow and red LEDs, the button and the potentiometer
 */
void initialiseGPIO()
{
    gpio_init(YELLOW_LED_PIN);
    gpio_set_dir(YELLOW_LED_PIN, GPIO_OUT);
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_down(BTN_PIN);
    // gpio_init(POT_PIN);
    adc_init();
    adc_gpio_init(POT_PIN);
    adc_select_input(0);
}

int main()
{
    stdio_init_all();
    initialiseGPIO();

    createTask(toggleYellowLED, NULL, 0, 1000);                 // Task 1 runs every 1000ms
    createTask(toggleRedLED, NULL, 0, 500);                     // Task 2 runs every 500ms
    createTask(printValue, NULL, 0, 2000);                      // Task 3 runs every 2000ms
    createTask(readPotentiometerIfButtonPressed, NULL, 0, 100); // Task 4 runs every 100ms

    scheduler(); // Start the scheduler

    return 0;
}
