/**
 * @file stm32f4xx.h
 * @brief STM32F4xx device specific header file
 * @author Fastbit Embedded Academy
 */

#ifndef STM32F4XX_H
#define STM32F4XX_H

#include <stdint.h>

/* Base addresses */
#define PERIPH_BASE           ((uint32_t)0x40000000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)

/* RCC Base Address */
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)

/* GPIO Base Addresses */
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)

/* Timer Base Addresses */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)

/* RCC Register Structure */
typedef struct
{
    volatile uint32_t CR;            /* RCC clock control register */
    volatile uint32_t PLLCFGR;       /* RCC PLL configuration register */
    volatile uint32_t CFGR;          /* RCC clock configuration register */
    volatile uint32_t CIR;           /* RCC clock interrupt register */
    volatile uint32_t AHB1RSTR;      /* RCC AHB1 peripheral reset register */
    volatile uint32_t AHB2RSTR;      /* RCC AHB2 peripheral reset register */
    volatile uint32_t AHB3RSTR;      /* RCC AHB3 peripheral reset register */
    volatile uint32_t RESERVED0;     /* Reserved */
    volatile uint32_t APB1RSTR;      /* RCC APB1 peripheral reset register */
    volatile uint32_t APB2RSTR;      /* RCC APB2 peripheral reset register */
    volatile uint32_t RESERVED1[2];  /* Reserved */
    volatile uint32_t AHB1ENR;       /* RCC AHB1 peripheral clock enable register */
    volatile uint32_t AHB2ENR;       /* RCC AHB2 peripheral clock enable register */
    volatile uint32_t AHB3ENR;       /* RCC AHB3 peripheral clock enable register */
    volatile uint32_t RESERVED2;     /* Reserved */
    volatile uint32_t APB1ENR;       /* RCC APB1 peripheral clock enable register */
    volatile uint32_t APB2ENR;       /* RCC APB2 peripheral clock enable register */
} RCC_TypeDef;

/* GPIO Register Structure */
typedef struct
{
    volatile uint32_t MODER;         /* GPIO port mode register */
    volatile uint32_t OTYPER;        /* GPIO port output type register */
    volatile uint32_t OSPEEDR;       /* GPIO port output speed register */
    volatile uint32_t PUPDR;         /* GPIO port pull-up/pull-down register */
    volatile uint32_t IDR;           /* GPIO port input data register */
    volatile uint32_t ODR;           /* GPIO port output data register */
    volatile uint32_t BSRR;          /* GPIO port bit set/reset register */
    volatile uint32_t LCKR;          /* GPIO port configuration lock register */
    volatile uint32_t AFR[2];        /* GPIO alternate function registers */
} GPIO_TypeDef;

/* Timer Register Structure */
typedef struct
{
    volatile uint32_t CR1;           /* TIM control register 1 */
    volatile uint32_t CR2;           /* TIM control register 2 */
    volatile uint32_t SMCR;          /* TIM slave mode control register */
    volatile uint32_t DIER;          /* TIM DMA/Interrupt enable register */
    volatile uint32_t SR;            /* TIM status register */
    volatile uint32_t EGR;           /* TIM event generation register */
    volatile uint32_t CCMR1;         /* TIM capture/compare mode register 1 */
    volatile uint32_t CCMR2;         /* TIM capture/compare mode register 2 */
    volatile uint32_t CCER;          /* TIM capture/compare enable register */
    volatile uint32_t CNT;           /* TIM counter register */
    volatile uint32_t PSC;           /* TIM prescaler register */
    volatile uint32_t ARR;           /* TIM auto-reload register */
    volatile uint32_t RESERVED0;     /* Reserved */
    volatile uint32_t CCR1;          /* TIM capture/compare register 1 */
    volatile uint32_t CCR2;          /* TIM capture/compare register 2 */
    volatile uint32_t CCR3;          /* TIM capture/compare register 3 */
    volatile uint32_t CCR4;          /* TIM capture/compare register 4 */
    volatile uint32_t RESERVED1;     /* Reserved */
    volatile uint32_t DCR;           /* TIM DMA control register */
    volatile uint32_t DMAR;          /* TIM DMA address for full transfer */
    volatile uint32_t OR;            /* TIM option register */
} TIM_TypeDef;

/* Peripheral declarations */
#define RCC                   ((RCC_TypeDef *)RCC_BASE)
#define GPIOA                 ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC                 ((GPIO_TypeDef *)GPIOC_BASE)
#define TIM2                  ((TIM_TypeDef *)TIM2_BASE)

/* RCC AHB1 peripheral clock enable register bits */
#define RCC_AHB1ENR_GPIOAEN   (1 << 0)
#define RCC_AHB1ENR_GPIOBEN   (1 << 1)
#define RCC_AHB1ENR_GPIOCEN   (1 << 2)

/* RCC APB1 peripheral clock enable register bits */
#define RCC_APB1ENR_TIM2EN    (1 << 0)

/* TIM control register 1 bits */
#define TIM_CR1_CEN           (1 << 0)

/* TIM DMA/Interrupt enable register bits */
#define TIM_DIER_UIE          (1 << 0)

/* TIM status register bits */
#define TIM_SR_UIF            (1 << 0)

/* IRQ Numbers */
typedef enum
{
    TIM2_IRQn = 28
} IRQn_Type;

/* NVIC Functions */
static inline void NVIC_EnableIRQ(IRQn_Type IRQn)
{
    volatile uint32_t *NVIC_ISER = (uint32_t *)0xE000E100;
    NVIC_ISER[IRQn >> 5] = (1 << (IRQn & 0x1F));
}

static inline void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
    volatile uint8_t *NVIC_IPR = (uint8_t *)0xE000E400;
    NVIC_IPR[IRQn] = priority << 4;
}

/* Compiler intrinsics */
#define __NOP() __asm__ volatile ("nop")

#endif /* STM32F4XX_H */
