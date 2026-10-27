/*
 * =============================================================================
 *  CHALLENGE 2 — SOLUTION
 * =============================================================================
 *  Wiring: LED on D4 (PB5) and D9 (PC7), each through a resistor to GND.
 *
 *  Where the numbers come from:
 *    - RM0390 Table 1: GPIOB = 0x4002 0400 - 0x4002 07FF
 *                      GPIOC = 0x4002 0800 - 0x4002 0BFF
 *    - Offsets are identical for every GPIO port (RM0390 7.4.11):
 *                      MODER = 0x00, ODR = 0x14
 *    - Clocks: RCC_AHB1ENR bit 1 = GPIOB, bit 2 = GPIOC
 * =============================================================================
 */
#include "esdc.h"

#define GPIOB_BASE    0x40020400U
#define GPIOB_MODER   (*(volatile uint32_t *)(GPIOB_BASE + 0x00U))
#define GPIOB_ODR     (*(volatile uint32_t *)(GPIOB_BASE + 0x14U))

#define GPIOC_BASE    0x40020800U
#define GPIOC_MODER   (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_ODR     (*(volatile uint32_t *)(GPIOC_BASE + 0x14U))

int main(void)
{
    RCC_AHB1ENR |= (1U << 1) | (1U << 2);     /* GPIOB + GPIOC clocks on */

    GPIOB_MODER &= ~(3U << (5U * 2U));        /* PB5 = output (01) */
    GPIOB_MODER |=  (1U << (5U * 2U));
    GPIOC_MODER &= ~(3U << (7U * 2U));        /* PC7 = output (01) */
    GPIOC_MODER |=  (1U << (7U * 2U));

    while (1) {
        GPIOB_ODR |=  (1U << 5);
        GPIOC_ODR &= ~(1U << 7);
        delay_ms(400);

        GPIOB_ODR &= ~(1U << 5);
        GPIOC_ODR |=  (1U << 7);
        delay_ms(400);
    }
}
