
# Minimal RTOS for Raspberry Pi Pico

A simple real-time operating system (RTOS) implementation using either a round-robin or priority-based scheduler designed for the Raspberry Pi Pico. This RTOS manages multiple tasks with adjustable periods and includes basic hardware interaction through GPIO and ADC.

## Features

- **Flexible Task Scheduling**: Choose between round-robin or priority-based scheduling to manage tasks.
- **GPIO control**: Toggle LEDs connected to specific GPIO pins.
- **ADC reading**: Reads analog values from a potentiometer through ADC.
- **Button interaction**: Triggers ADC reading when a button is pressed.
- **UART communication**: Prints the last recorded value of the potentiometer.

## Prerequisites

- Raspberry Pi Pico SDK
- C/C++ compiler set up for Raspberry Pi Pico (like arm-none-eabi-gcc)
- Hardware setup as per the schematic provided

## Environment

This project was built using Ubuntu 22.04.3 Windows Subsystem for Linux. This project should build under a standard linux distribution. 


## Hardware Setup

Refer to the following schematic to connect LEDs, a button, and a potentiometer to your Raspberry Pi Pico:

![Schematic](https://raw.githubusercontent.com/gcoulby/minimal-rtos/main/Schematic_MinimalRTOS.svg)

## Usage

1. **Clone the repository**:
   ```bash
   git clone https://github.com/gcoulby/minimal-rtos.git
   cd minimal-rtos
   ```

2. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

   ***My preferred method - use a one-liner***

   ```bash
   cmake -S . -B build && cmake --build build
   ```

3. **Upload the firmware** to your Raspberry Pi Pico.

4. **Restart the Pico** to run the uploaded firmware.

## Task Manager API

### Initialization

- `TaskManager(SchedulerType schedulerType)`: Initialize the Task Manager with a specified scheduling type (ROUND_ROBIN or PRIORITY_BASED).

### Task Creation

- `void createTask(TaskFunction_t function, void *params, uint32_t delay, uint32_t period, TaskPriority priority)`: Schedule a new task with specific timing and priority.

### Scheduler Start

- `void scheduler()`: Starts the task scheduler, managing the execution of all active tasks.

## Example Tasks

- `toggleYellowLED`: Toggles the yellow LED state.
- `toggleRedLED`: Toggles the red LED state.
- `printValue`: Prints the current value of the potentiometer and the calculated voltage.
- `readPotentiometerIfButtonPressed`: Checks if the button is pressed and reads the potentiometer value if true.

## Contributing

Contributions are welcome. Please fork the repository and submit a pull request with your changes.

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Authors

- **Graham Coulby** - *Initial work* - [gcoulby](https://github.com/gcoulby)
