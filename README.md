# STM32 Productivity Timer

A Pomodoro-style productivity timer firmware for STM32F4 microcontrollers, developed by Fastbit Embedded Academy.

## Overview

This project implements a productivity timer using the Pomodoro Technique on an STM32F407VG microcontroller. The timer helps manage work sessions and breaks to improve productivity.

### Features

- **25-minute work sessions** (Pomodoro standard)
- **5-minute short breaks** after each work session
- **15-minute long breaks** after every 4 work sessions
- **LED indicators** for work and break periods
- **Button control** for starting sessions
- **Hardware timer-based** accurate timing

## Hardware Requirements

- STM32F407VG microcontroller (or compatible STM32F4 series)
- 2 LEDs (for work and break indication)
- 1 Push button (for starting/stopping timer)
- ST-Link programmer (for flashing)

### Pin Configuration

- **PA5**: Work session LED
- **PA6**: Break session LED
- **PC13**: Start/Stop button (with pull-up)

## Software Requirements

- ARM GCC toolchain (`arm-none-eabi-gcc`)
- Make
- ST-Link tools (`st-flash` for programming)

## Project Structure

```
fw-stm32-productivity-timer/
├── src/
│   ├── main.c                  # Main application logic
│   ├── timer.c                 # Timer peripheral implementation
│   ├── gpio.c                  # GPIO configuration for LEDs and buttons
│   ├── system_stm32f4xx.c      # System initialization
│   └── startup_stm32f4xx.c     # Startup code and vector table
├── inc/
│   ├── stm32f4xx.h            # STM32F4 device definitions
│   ├── timer.h                # Timer header
│   ├── gpio.h                 # GPIO header
│   └── config.h               # Configuration constants
├── build/                      # Build output directory
├── STM32F407VG.ld             # Linker script
├── Makefile                   # Build configuration
└── README.md                  # This file
```

## Building the Project

1. **Install ARM GCC Toolchain**:
   ```bash
   # On Ubuntu/Debian
   sudo apt-get install gcc-arm-none-eabi
   ```

2. **Build the firmware**:
   ```bash
   make
   ```

3. **Clean build files**:
   ```bash
   make clean
   ```

## Flashing the Firmware

1. **Connect your ST-Link programmer** to the STM32 board

2. **Flash the firmware**:
   ```bash
   make flash
   ```

   Or manually:
   ```bash
   st-flash write build/productivity_timer.bin 0x08000000
   ```

## Usage

1. **Power on** the STM32 board
2. **Press the button** (PC13) to start a work session
3. **Work LED** (PA5) will light up during the 25-minute work session
4. After the work session, the **Break LED** (PA6) will light up
5. The timer automatically alternates between work sessions and breaks
6. After every 4 work sessions, you get a 15-minute long break

## Timer States

- **IDLE**: Waiting for button press to start
- **WORK**: 25-minute work session in progress
- **SHORT_BREAK**: 5-minute break after work session
- **LONG_BREAK**: 15-minute break after 4 work sessions

## How It Works

The timer uses the STM32's TIM2 peripheral to generate accurate 1-second interrupts. The system currently runs on the default HSI clock (16 MHz), which provides sufficient accuracy for the timer application. The main application state machine manages transitions between work and break periods based on the Pomodoro Technique.

**Note**: For production applications requiring higher performance, the system can be configured to use the PLL for higher clock speeds (up to 168 MHz). See `src/system_stm32f4xx.c` for PLL configuration instructions.

### Pomodoro Technique

1. Work for 25 minutes
2. Take a 5-minute break
3. After 4 work sessions, take a 15-minute break
4. Repeat

## Customization

You can customize the timer durations by modifying the values in `inc/config.h`:

```c
#define WORK_SESSION_DURATION       25    /* Work session (minutes) */
#define SHORT_BREAK_DURATION        5     /* Short break (minutes) */
#define LONG_BREAK_DURATION         15    /* Long break (minutes) */
#define SESSIONS_BEFORE_LONG_BREAK  4     /* Sessions before long break */
```

You can also customize the pin assignments:

```c
#define LED_WORK_PORT               GPIOA
#define LED_WORK_PIN                5
#define LED_BREAK_PORT              GPIOA
#define LED_BREAK_PIN               6
#define BUTTON_PORT                 GPIOC
#define BUTTON_PIN                  13
```

## About Fastbit Embedded Academy

This project is developed by Fastbit Embedded Academy for teaching embedded systems development with STM32 microcontrollers.

## License

This project is provided as-is for educational purposes.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Contact

For questions or support, please contact Fastbit Embedded Academy.