/**
 * @file config.h
 * @brief Configuration file for productivity timer
 * @author Fastbit Embedded Academy
 */

#ifndef CONFIG_H
#define CONFIG_H

/* Timer Configuration (in minutes) */
#define WORK_SESSION_DURATION       25    /* Pomodoro work session */
#define SHORT_BREAK_DURATION        5     /* Short break duration */
#define LONG_BREAK_DURATION         15    /* Long break duration */
#define SESSIONS_BEFORE_LONG_BREAK  4     /* Number of work sessions before long break */

/* Hardware Configuration */
#define LED_WORK_PORT               GPIOA
#define LED_WORK_PIN                5

#define LED_BREAK_PORT              GPIOA
#define LED_BREAK_PIN               6

#define BUTTON_PORT                 GPIOC
#define BUTTON_PIN                  13

/* System Clock Configuration */
/* Note: Current implementation uses HSI (16 MHz) as system clock */
/* For production, configure PLL to achieve higher frequencies */
#define SYSTEM_CLOCK_HZ             16000000   /* 16 MHz HSI */
#define APB1_CLOCK_HZ               16000000   /* Same as system clock without prescaler */

/* Timer Configuration */
#define TIMER_PRESCALER             ((APB1_CLOCK_HZ / 1000) - 1)  /* 1 kHz timer clock */
#define TIMER_PERIOD                (1000 - 1)                     /* 1 second period */

/* Delay Configuration */
#define DELAY_CYCLES_PER_MS         (SYSTEM_CLOCK_HZ / 1000 / 4)  /* Approximate cycles for 1ms delay */

#endif /* CONFIG_H */
