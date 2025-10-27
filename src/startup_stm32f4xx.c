/**
 * @file startup_stm32f4xx.c
 * @brief Startup code for STM32F4xx microcontroller
 * @author Fastbit Embedded Academy
 */

#include <stdint.h>

/* External declarations */
extern int main(void);
extern void SystemInit(void);

/* Stack top (end of RAM) */
extern uint32_t _estack;

/* Linker script symbols */
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* Default handler */
void Default_Handler(void);

/* Reset handler */
void Reset_Handler(void);

/* Cortex-M4 core handlers */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* STM32 peripheral handlers */
void TIM2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

/* Vector table */
__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack),
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    
    /* External Interrupts */
    Default_Handler,   /* WWDG_IRQHandler */
    Default_Handler,   /* PVD_IRQHandler */
    Default_Handler,   /* TAMP_STAMP_IRQHandler */
    Default_Handler,   /* RTC_WKUP_IRQHandler */
    Default_Handler,   /* FLASH_IRQHandler */
    Default_Handler,   /* RCC_IRQHandler */
    Default_Handler,   /* EXTI0_IRQHandler */
    Default_Handler,   /* EXTI1_IRQHandler */
    Default_Handler,   /* EXTI2_IRQHandler */
    Default_Handler,   /* EXTI3_IRQHandler */
    Default_Handler,   /* EXTI4_IRQHandler */
    Default_Handler,   /* DMA1_Stream0_IRQHandler */
    Default_Handler,   /* DMA1_Stream1_IRQHandler */
    Default_Handler,   /* DMA1_Stream2_IRQHandler */
    Default_Handler,   /* DMA1_Stream3_IRQHandler */
    Default_Handler,   /* DMA1_Stream4_IRQHandler */
    Default_Handler,   /* DMA1_Stream5_IRQHandler */
    Default_Handler,   /* DMA1_Stream6_IRQHandler */
    Default_Handler,   /* ADC_IRQHandler */
    Default_Handler,   /* CAN1_TX_IRQHandler */
    Default_Handler,   /* CAN1_RX0_IRQHandler */
    Default_Handler,   /* CAN1_RX1_IRQHandler */
    Default_Handler,   /* CAN1_SCE_IRQHandler */
    Default_Handler,   /* EXTI9_5_IRQHandler */
    Default_Handler,   /* TIM1_BRK_TIM9_IRQHandler */
    Default_Handler,   /* TIM1_UP_TIM10_IRQHandler */
    Default_Handler,   /* TIM1_TRG_COM_TIM11_IRQHandler */
    Default_Handler,   /* TIM1_CC_IRQHandler */
    TIM2_IRQHandler,   /* TIM2_IRQHandler */
};

/**
 * @brief Reset handler - Entry point after reset
 */
void Reset_Handler(void)
{
    uint32_t *src, *dst;

    /* Copy data section from Flash to RAM */
    src = &_sidata;
    dst = &_sdata;
    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    /* Zero fill bss section */
    dst = &_sbss;
    while (dst < &_ebss)
    {
        *dst++ = 0;
    }

    /* Call system initialization function */
    SystemInit();

    /* Call the application's entry point */
    main();

    /* Infinite loop if main returns */
    while (1);
}

/**
 * @brief Default handler for unused interrupts
 */
void Default_Handler(void)
{
    /* Infinite loop */
    while (1);
}
