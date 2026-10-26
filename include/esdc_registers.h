/*
 * =============================================================================
 *  esdc_registers.h  —  Register definitions for the STM32F446RE
 * =============================================================================
 *
 *  A register is a 32-bit spot in memory that is wired to hardware.
 *  Writing to it changes the hardware (turns a pin on). Reading it tells you
 *  what the hardware is doing (is the button pressed?).
 *
 *  Every register lives at:   BASE ADDRESS of its peripheral  +  OFFSET
 *
 *      Base addresses  ->  RM0390, Section 2.2.2 "Memory map", Table 1
 *      Offsets         ->  RM0390, the "register map" table at the end of
 *                          each peripheral's chapter (GPIO = Section 7.4.11,
 *                          RCC = the register map at the end of Section 6.3)
 *
 *  RM0390 is the STM32F446 Reference Manual. Search "RM0390 pdf".
 *
 *  ---------------------------------------------------------------------------
 *  How to read one of these lines (right to left):
 *
 *      #define GPIOA_ODR  (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))
 *                                                 ^^^^^^^^^^^^^^^^^^^^
 *                                                 1. the address (a number)
 *                            ^^^^^^^^^^^^^^^^^^^^
 *                            2. "treat that number as the address of a
 *                               32-bit value that hardware can change"
 *                         ^
 *                         3. "go to that address" (dereference)
 *
 *  End result: GPIOA_ODR behaves like a normal variable. You can write
 *  GPIOA_ODR |= (1U << 5); and it changes the real pin.
 *
 *  You do NOT need to understand the pointer part to finish the challenges.
 *  Just treat these names like variables.
 * =============================================================================
 */
#ifndef ESDC_REGISTERS_H
#define ESDC_REGISTERS_H

#include <stdint.h>   /* gives us uint32_t = "exactly 32 bits, unsigned" */

/* -----------------------------------------------------------------------------
 *  RCC — Reset and Clock Control
 *
 *  Every peripheral starts with its clock OFF to save power. A GPIO port with
 *  its clock off ignores everything you write to it. Turning the clock on is
 *  ALWAYS step zero.
 *
 *  Think of it as the circuit breaker for a building: flip it before you
 *  touch anything inside.
 * -------------------------------------------------------------------------- */
#define RCC_BASE        0x40023800U

/* AHB1 peripheral clock enable register (RM0390 6.3.10)
 *   bit 0 = GPIOA,  bit 1 = GPIOB,  bit 2 = GPIOC,  bit 3 = GPIOD ... */
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30U))

/* APB1 peripheral clock enable register (RM0390 6.3.13)
 *   bit 0 = TIM2,  bit 1 = TIM3 ...  (used in Challenge 4) */
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40U))

/* -----------------------------------------------------------------------------
 *  GPIOA — General Purpose I/O, Port A
 *
 *  Pin PA5 is the green LED on the Nucleo board (labelled LD2).
 *
 *  Each GPIO port has the SAME set of registers at the SAME offsets.
 *  Only the base address changes between GPIOA, GPIOB, GPIOC...
 *  (That's a big hint for Challenge 2.)
 * -------------------------------------------------------------------------- */
#define GPIOA_BASE      0x40020000U

/* Mode register: 2 bits per pin.  00 = input, 01 = output,
 *                                  10 = alternate function, 11 = analog
 * Pin n uses bits (2n+1):(2n).  Example: PA5 uses bits 11:10.            */
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))

/* Pull-up / pull-down register: 2 bits per pin. 00 none, 01 up, 10 down */
#define GPIOA_PUPDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))

/* Input data register: bit n = what pin n is reading right now (0 or 1) */
#define GPIOA_IDR       (*(volatile uint32_t *)(GPIOA_BASE + 0x10U))

/* Output data register: bit n = what pin n is driving (1 = high = LED on) */
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))

/* -----------------------------------------------------------------------------
 *  GPIOB and GPIOC are NOT defined here on purpose.
 *  Finding them in the reference manual is part of Challenge 2.
 *  (Challenge 3 and 4 give you the ones they need at the top of their files.)
 * -------------------------------------------------------------------------- */

#endif /* ESDC_REGISTERS_H */
