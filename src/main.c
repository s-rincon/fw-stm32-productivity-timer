/**
 * @file main.c
 * @brief Main application for STM32 Productivity Timer
 * @author Fastbit Embedded Academy
 * @description Pomodoro-style productivity timer for STM32 microcontroller
 */

#include "stm32f4xx.h"
#include "timer.h"
#include "gpio.h"

/* Timer states */
typedef enum {
    TIMER_IDLE,
    TIMER_WORK,
    TIMER_SHORT_BREAK,
    TIMER_LONG_BREAK
} TimerState_t;

/* Global variables */
static TimerState_t currentState = TIMER_IDLE;
static uint32_t workSessions = 0;

/* Function prototypes */
void SystemInit(void);
void delay_ms(uint32_t ms);

/**
 * @brief Main function
 */
int main(void)
{
    /* Initialize system clock */
    SystemInit();
    
    /* Initialize GPIO for LEDs and buttons */
    GPIO_Init();
    
    /* Initialize timer */
    Timer_Init();
    
    /* Main loop */
    while(1)
    {
        switch(currentState)
        {
            case TIMER_IDLE:
                /* Wait for button press to start work session */
                if(GPIO_ReadButton())
                {
                    currentState = TIMER_WORK;
                    Timer_Start(25); /* 25 minutes work session */
                    GPIO_SetLED(1); /* Turn on work LED */
                }
                break;
                
            case TIMER_WORK:
                /* Check if work session is complete */
                if(Timer_IsComplete())
                {
                    workSessions++;
                    GPIO_SetLED(0); /* Turn off work LED */
                    
                    /* Decide on break type */
                    if(workSessions % 4 == 0)
                    {
                        currentState = TIMER_LONG_BREAK;
                        Timer_Start(15); /* 15 minutes long break */
                    }
                    else
                    {
                        currentState = TIMER_SHORT_BREAK;
                        Timer_Start(5); /* 5 minutes short break */
                    }
                    GPIO_SetLED(2); /* Turn on break LED */
                }
                break;
                
            case TIMER_SHORT_BREAK:
            case TIMER_LONG_BREAK:
                /* Check if break is complete */
                if(Timer_IsComplete())
                {
                    GPIO_SetLED(0); /* Turn off break LED */
                    currentState = TIMER_IDLE;
                }
                break;
                
            default:
                currentState = TIMER_IDLE;
                break;
        }
        
        /* Small delay to prevent busy waiting */
        delay_ms(100);
    }
    
    return 0;
}

/**
 * @brief Simple delay function
 * @param ms Delay time in milliseconds
 */
void delay_ms(uint32_t ms)
{
    /* Assuming 168 MHz system clock */
    for(uint32_t i = 0; i < ms * 42000; i++)
    {
        __NOP();
    }
}
