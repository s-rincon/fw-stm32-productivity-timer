/**
 * @file timer.c
 * @brief Timer implementation for productivity timer
 * @author Fastbit Embedded Academy
 */

#include "timer.h"
#include "stm32f4xx.h"
#include "config.h"

static uint32_t timerMinutes = 0;
static uint32_t timerSeconds = 0;
static uint8_t timerRunning = 0;

/**
 * @brief Initialize timer peripheral
 */
void Timer_Init(void)
{
    /* Enable TIM2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    
    /* Configure TIM2 for 1 second interrupts
     * Timer calculation: 
     * - APB1 clock is divided by (PSC + 1) to get timer clock
     * - Timer overflows when CNT reaches (ARR + 1)
     * - Total period = (PSC + 1) * (ARR + 1) / APB1_CLOCK_HZ
     * - Example with current config: (TIMER_PRESCALER + 1) * (TIMER_PERIOD + 1) / APB1_CLOCK_HZ
     *   = 16000 * 1000 / 16000000 = 1 second
     */
    TIM2->PSC = TIMER_PRESCALER;  /* Prescaler for 1 kHz timer clock */
    TIM2->ARR = TIMER_PERIOD;     /* Auto-reload for 1 second period */
    
    /* Enable update interrupt */
    TIM2->DIER |= TIM_DIER_UIE;
    
    /* Enable TIM2 interrupt in NVIC */
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0);
    
    timerRunning = 0;
    timerMinutes = 0;
    timerSeconds = 0;
}

/**
 * @brief Start timer with specified minutes
 * @param minutes Duration in minutes
 */
void Timer_Start(uint32_t minutes)
{
    timerMinutes = minutes;
    timerSeconds = 0;
    timerRunning = 1;
    
    /* Enable timer */
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief Stop timer
 */
void Timer_Stop(void)
{
    timerRunning = 0;
    
    /* Disable timer */
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief Check if timer is complete
 * @return 1 if complete, 0 otherwise
 */
uint8_t Timer_IsComplete(void)
{
    if(!timerRunning && timerMinutes == 0 && timerSeconds == 0)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Get remaining minutes
 * @return Remaining minutes
 */
uint32_t Timer_GetMinutes(void)
{
    return timerMinutes;
}

/**
 * @brief Get remaining seconds
 * @return Remaining seconds
 */
uint32_t Timer_GetSeconds(void)
{
    return timerSeconds;
}

/**
 * @brief TIM2 interrupt handler
 */
void TIM2_IRQHandler(void)
{
    if(TIM2->SR & TIM_SR_UIF)
    {
        /* Clear interrupt flag */
        TIM2->SR &= ~TIM_SR_UIF;
        
        if(timerRunning)
        {
            if(timerSeconds > 0)
            {
                timerSeconds--;
            }
            else if(timerMinutes > 0)
            {
                timerMinutes--;
                timerSeconds = 59;
            }
            else
            {
                /* Timer completed */
                timerRunning = 0;
                TIM2->CR1 &= ~TIM_CR1_CEN;
            }
        }
    }
}
