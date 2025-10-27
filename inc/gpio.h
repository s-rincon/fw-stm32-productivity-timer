/**
 * @file gpio.h
 * @brief GPIO header file for LEDs and buttons
 * @author Fastbit Embedded Academy
 */

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/**
 * @brief Initialize GPIO for LEDs and buttons
 */
void GPIO_Init(void);

/**
 * @brief Set LED state
 * @param led LED number (1 = work LED, 2 = break LED, 0 = all off)
 */
void GPIO_SetLED(uint8_t led);

/**
 * @brief Toggle LED
 * @param led LED number (1 = work LED, 2 = break LED)
 */
void GPIO_ToggleLED(uint8_t led);

/**
 * @brief Read button state
 * @return 1 if button is pressed, 0 otherwise
 */
uint8_t GPIO_ReadButton(void);

#endif /* GPIO_H */
