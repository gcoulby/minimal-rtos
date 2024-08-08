/**
 * @file rtos.c
 * @brief A minimal RTOS using a round-robin scheduler for Pico
 * @author Graham Coulby
 * @date 2024-08-08
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h" // Include this header for ADC functions
#include <taskManager.h>  // Include the task manager header file

#define YELLOW_LED_PIN 18
#define RED_LED_PIN 19
#define BTN_PIN 20
#define POT_PIN 26

static bool led1_state = false;
static bool led2_state = false;
static int pot_value = 0;

TaskManager taskManager = TaskManager(PRIORITY_BASED);

/*==================[Task Functions]==================*/
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
/*==================[END Task Functions]==================*/

/**
 * @brief This function initialises the GPIO pins
 * @note This initialises the yellow and red LEDs, the button and the potentiometer
 */
void initialiseGPIO()
{
    // initialise the GPIO pins
    gpio_init(YELLOW_LED_PIN);
    gpio_set_dir(YELLOW_LED_PIN, GPIO_OUT);
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_down(BTN_PIN);
    // Initialise the ADC
    adc_init();
    adc_gpio_init(POT_PIN);
    adc_select_input(0);
}

int main()
{
    stdio_init_all();
    initialiseGPIO();

    taskManager.createTask(toggleYellowLED, NULL, 0, 1000, MEDIUM_PRIORITY);               // Task 1 runs every 1000ms
    taskManager.createTask(toggleRedLED, NULL, 0, 500, MEDIUM_PRIORITY);                   // Task 2 runs every 500ms
    taskManager.createTask(printValue, NULL, 0, 2000);                                     // Task 3 runs every 2000ms
    taskManager.createTask(readPotentiometerIfButtonPressed, NULL, 0, 100, HIGH_PRIORITY); // Task 4 runs every 100ms

    taskManager.scheduler(); // Start the scheduler

    return 0;
}
