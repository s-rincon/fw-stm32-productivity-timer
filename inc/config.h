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
#define SYSTEM_CLOCK_MHZ            168
#define APB1_CLOCK_MHZ              84

#endif /* CONFIG_H */
