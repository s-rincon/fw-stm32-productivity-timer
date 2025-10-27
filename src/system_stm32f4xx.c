/**
 * @file system_stm32f4xx.c
 * @brief System initialization for STM32F4xx
 * @author Fastbit Embedded Academy
 */

#include "stm32f4xx.h"

/**
 * @brief System initialization function
 */
void SystemInit(void)
{
    /* FPU settings - Enable FPU */
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
        volatile uint32_t *CPACR = (uint32_t *)0xE000ED88;
        *CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* Set CP10 and CP11 Full Access */
    #endif
    
    /* Reset the RCC clock configuration to the default reset state */
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;

    /* Reset CFGR register */
    RCC->CFGR = 0x00000000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    /* Reset PLLCFGR register */
    RCC->PLLCFGR = 0x24003010;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000;

    /* Configure the System clock source, PLL Multiplier and Divider factors, 
       AHB/APBx prescalers and Flash settings */
    /* This can be enhanced to configure PLL for higher clock speeds */
}
