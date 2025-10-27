/**
 * @file gpio.c
 * @brief GPIO implementation for LEDs and buttons
 * @author Fastbit Embedded Academy
 */

#include "gpio.h"
#include "stm32f4xx.h"

/* LED pins - Using GPIOA */
#define LED_WORK_PIN      5  /* PA5 - Work session LED */
#define LED_BREAK_PIN     6  /* PA6 - Break session LED */

/* Button pin - Using GPIOC */
#define BUTTON_PIN        13 /* PC13 - Start/Stop button */

/**
 * @brief Initialize GPIO for LEDs and buttons
 */
void GPIO_Init(void)
{
    /* Enable GPIOA and GPIOC clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
    
    /* Configure LED pins as output */
    GPIOA->MODER |= (1 << (LED_WORK_PIN * 2));  /* Output mode for PA5 */
    GPIOA->MODER |= (1 << (LED_BREAK_PIN * 2)); /* Output mode for PA6 */
    
    /* Configure output type as push-pull */
    GPIOA->OTYPER &= ~(1 << LED_WORK_PIN);
    GPIOA->OTYPER &= ~(1 << LED_BREAK_PIN);
    
    /* Configure output speed as high */
    GPIOA->OSPEEDR |= (3 << (LED_WORK_PIN * 2));
    GPIOA->OSPEEDR |= (3 << (LED_BREAK_PIN * 2));
    
    /* No pull-up/pull-down */
    GPIOA->PUPDR &= ~(3 << (LED_WORK_PIN * 2));
    GPIOA->PUPDR &= ~(3 << (LED_BREAK_PIN * 2));
    
    /* Configure button pin as input */
    GPIOC->MODER &= ~(3 << (BUTTON_PIN * 2)); /* Input mode for PC13 */
    
    /* Configure with pull-up */
    GPIOC->PUPDR |= (1 << (BUTTON_PIN * 2));
    
    /* Initialize LEDs to OFF */
    GPIOA->ODR &= ~(1 << LED_WORK_PIN);
    GPIOA->ODR &= ~(1 << LED_BREAK_PIN);
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
        GPIOA->ODR &= ~(1 << LED_WORK_PIN);
        GPIOA->ODR &= ~(1 << LED_BREAK_PIN);
    }
    else if(led == 1)
    {
        /* Turn on work LED, turn off break LED */
        GPIOA->ODR |= (1 << LED_WORK_PIN);
        GPIOA->ODR &= ~(1 << LED_BREAK_PIN);
    }
    else if(led == 2)
    {
        /* Turn off work LED, turn on break LED */
        GPIOA->ODR &= ~(1 << LED_WORK_PIN);
        GPIOA->ODR |= (1 << LED_BREAK_PIN);
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
        GPIOA->ODR ^= (1 << LED_WORK_PIN);
    }
    else if(led == 2)
    {
        GPIOA->ODR ^= (1 << LED_BREAK_PIN);
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
    uint8_t currentState = (GPIOC->IDR & (1 << BUTTON_PIN)) ? 1 : 0;
    
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
