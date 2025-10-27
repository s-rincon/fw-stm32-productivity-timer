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

    /* Note: System clock configuration with PLL
     * Currently using default HSI (16 MHz) clock
     * For production use, configure PLL to achieve 168 MHz:
     * 
     * Example PLL configuration for 168 MHz from HSI (16 MHz):
     * - PLLM = 16 (16 MHz / 16 = 1 MHz VCO input)
     * - PLLN = 336 (1 MHz * 336 = 336 MHz VCO output)
     * - PLLP = 2 (336 MHz / 2 = 168 MHz system clock)
     * - PLLQ = 7 (336 MHz / 7 = 48 MHz for USB)
     * 
     * Uncomment and adapt the following for PLL configuration:
     * RCC->PLLCFGR = (16 << 0) | (336 << 6) | (0 << 16) | (7 << 24);
     * RCC->CR |= RCC_CR_PLLON;
     * while(!(RCC->CR & RCC_CR_PLLRDY));
     * RCC->CFGR |= RCC_CFGR_SW_PLL;
     * while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
     */
}
