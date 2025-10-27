/**
 * @file gpio.c
 * @brief GPIO implementation for LEDs and buttons
 * @author Fastbit Embedded Academy
 */

#include "gpio.h"
#include "stm32f4xx.h"
#include "config.h"

/**
 * @brief Initialize GPIO for LEDs and buttons
 */
void GPIO_Init(void)
{
    /* Enable GPIOA and GPIOC clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
    
    /* Configure LED pins as output */
    LED_WORK_PORT->MODER |= (1 << (LED_WORK_PIN * 2));  /* Output mode */
    LED_BREAK_PORT->MODER |= (1 << (LED_BREAK_PIN * 2)); /* Output mode */
    
    /* Configure output type as push-pull */
    LED_WORK_PORT->OTYPER &= ~(1 << LED_WORK_PIN);
    LED_BREAK_PORT->OTYPER &= ~(1 << LED_BREAK_PIN);
    
    /* Configure output speed as high */
    LED_WORK_PORT->OSPEEDR |= (3 << (LED_WORK_PIN * 2));
    LED_BREAK_PORT->OSPEEDR |= (3 << (LED_BREAK_PIN * 2));
    
    /* No pull-up/pull-down */
    LED_WORK_PORT->PUPDR &= ~(3 << (LED_WORK_PIN * 2));
    LED_BREAK_PORT->PUPDR &= ~(3 << (LED_BREAK_PIN * 2));
    
    /* Configure button pin as input */
    BUTTON_PORT->MODER &= ~(3 << (BUTTON_PIN * 2)); /* Input mode */
    
    /* Configure with pull-up */
    BUTTON_PORT->PUPDR |= (1 << (BUTTON_PIN * 2));
    
    /* Initialize LEDs to OFF */
    LED_WORK_PORT->ODR &= ~(1 << LED_WORK_PIN);
    LED_BREAK_PORT->ODR &= ~(1 << LED_BREAK_PIN);
}

/**
 * @brief Set LED state
 * @param led LED number (1 = work LED, 2 = break LED, 0 = all off)
 */
void GPIO_SetLED(uint8_t led)
{
    if(led == 0)
    {
        /* Turn off all LEDs */
        LED_WORK_PORT->ODR &= ~(1 << LED_WORK_PIN);
        LED_BREAK_PORT->ODR &= ~(1 << LED_BREAK_PIN);
    }
    else if(led == 1)
    {
        /* Turn on work LED, turn off break LED */
        LED_WORK_PORT->ODR |= (1 << LED_WORK_PIN);
        LED_BREAK_PORT->ODR &= ~(1 << LED_BREAK_PIN);
    }
    else if(led == 2)
    {
        /* Turn off work LED, turn on break LED */
        LED_WORK_PORT->ODR &= ~(1 << LED_WORK_PIN);
        LED_BREAK_PORT->ODR |= (1 << LED_BREAK_PIN);
    }
}

/**
 * @brief Toggle LED
 * @param led LED number (1 = work LED, 2 = break LED)
 */
void GPIO_ToggleLED(uint8_t led)
{
    if(led == 1)
    {
        LED_WORK_PORT->ODR ^= (1 << LED_WORK_PIN);
    }
    else if(led == 2)
    {
        LED_BREAK_PORT->ODR ^= (1 << LED_BREAK_PIN);
    }
}

/**
 * @brief Read button state
 * @return 1 if button is pressed, 0 otherwise
 */
uint8_t GPIO_ReadButton(void)
{
    static uint8_t lastState = 1;
    static uint32_t debounceCounter = 0;
    uint8_t currentState = (BUTTON_PORT->IDR & (1 << BUTTON_PIN)) ? 1 : 0;
    
    /* Simple debouncing */
    if(currentState != lastState)
    {
        debounceCounter++;
        if(debounceCounter >= 10)
        {
            lastState = currentState;
            debounceCounter = 0;
            
            /* Return 1 on falling edge (button press) */
            if(currentState == 0)
            {
                return 1;
            }
        }
    }
    else
    {
        debounceCounter = 0;
    }
    
    return 0;
}
