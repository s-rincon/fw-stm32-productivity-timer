/**
 * @file main.c
 * @brief Main application for STM32 Productivity Timer
 * @author Fastbit Embedded Academy
 * @description Pomodoro-style productivity timer for STM32 microcontroller
 */

#include "stm32f4xx.h"
#include "timer.h"
#include "gpio.h"
#include "config.h"

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
                    Timer_Start(WORK_SESSION_DURATION);
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
                    if(workSessions % SESSIONS_BEFORE_LONG_BREAK == 0)
                    {
                        currentState = TIMER_LONG_BREAK;
                        Timer_Start(LONG_BREAK_DURATION);
                    }
                    else
                    {
                        currentState = TIMER_SHORT_BREAK;
                        Timer_Start(SHORT_BREAK_DURATION);
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
 * @note This is an approximate delay based on the system clock
 */
void delay_ms(uint32_t ms)
{
    /* Using configured delay cycles per millisecond
     * Note: Actual delay may vary due to compiler optimizations
     * For precise delays, use hardware timers
     */
    for(uint32_t i = 0; i < ms * DELAY_CYCLES_PER_MS; i++)
    {
        __NOP();
    }
}
