/**
 * @file timer.h
 * @brief Timer header file for productivity timer
 * @author Fastbit Embedded Academy
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/**
 * @brief Initialize timer peripheral
 */
void Timer_Init(void);

/**
 * @brief Start timer with specified minutes
 * @param minutes Duration in minutes
 */
void Timer_Start(uint32_t minutes);

/**
 * @brief Stop timer
 */
void Timer_Stop(void);

/**
 * @brief Check if timer is complete
 * @return 1 if complete, 0 otherwise
 */
uint8_t Timer_IsComplete(void);

/**
 * @brief Get remaining minutes
 * @return Remaining minutes
 */
uint32_t Timer_GetMinutes(void);

/**
 * @brief Get remaining seconds
 * @return Remaining seconds
 */
uint32_t Timer_GetSeconds(void);

/**
 * @brief TIM2 interrupt handler
 */
void TIM2_IRQHandler(void);

#endif /* TIMER_H */
