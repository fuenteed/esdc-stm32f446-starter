/*
 * =============================================================================
 *  CHALLENGE 2 — Pins on OTHER ports   (skill: finding registers in the manual)
 * =============================================================================
 *
 *  WIRING (2 LEDs, same way as Challenge 1: pin -> LED -> resistor -> GND):
 *
 *      LED X:  D4  (= PB5)    <- Port B
 *      LED Y:  D9  (= PC7)    <- Port C
 *
 *  So far we only have Port A registers (in include/esdc_registers.h).
 *  This time YOU define the Port B and Port C registers below.
 *
 *  This file will NOT compile until you replace every FILL_ME_IN.
 *  That's on purpose: the compiler error points you at what's left.
 *
 *    STEP A:  fill in the #defines below (base addresses + offsets)
 *    STEP B:  turn on the clocks for GPIOB and GPIOC in main(), then make
 *             PB5 and PC7 outputs (same MODER shifts as Challenge 1)
 *    STEP C:  make the two LEDs alternate like a railroad crossing:
 *             X on / Y off, wait 400 ms, X off / Y on, wait 400 ms, repeat
 *             DONE WHEN: the two LEDs take turns.
 *
 *    STRETCH: rewire your 4 Challenge 1 LEDs too and run a 6-LED chase
 *             that crosses all three ports.
 *
 *  HINTS — read one at a time, only if stuck:
 *
 *    Hint 1:  Base addresses are in RM0390 Section 2.2.2, Table 1
 *             ("STM32F446xx register boundary addresses"). Find the rows
 *             for GPIOB and GPIOC. Use the START of the range.
 *             Remove any spaces: "0x4002 0400" -> 0x40020400
 *
 *    Hint 2:  Open include/esdc_registers.h and look at GPIOA's offsets.
 *             Every GPIO port uses the SAME offsets. Only the base changes.
 *
 *    Hint 3:  Clock enable bits in RCC_AHB1ENR: GPIOA = bit 0,
 *             GPIOB = bit 1, GPIOC = bit 2.  If your code builds and runs
 *             but the LED stays dark, it's almost always the clock.
 * =============================================================================
 */
#include "esdc.h"

/* ---- STEP A: your register definitions -------------------------------------
 *  Replace each FILL_ME_IN with a number. Keep the U at the end of numbers,
 *  for example 0x14U.                                                       */

#define GPIOB_BASE    FILL_ME_IN_base_address_see_RM0390_Table_1
#define GPIOB_MODER   (*(volatile uint32_t *)(GPIOB_BASE + FILL_ME_IN_offset))
#define GPIOB_ODR     (*(volatile uint32_t *)(GPIOB_BASE + FILL_ME_IN_offset))

#define GPIOC_BASE    FILL_ME_IN_base_address_see_RM0390_Table_1
#define GPIOC_MODER   (*(volatile uint32_t *)(GPIOC_BASE + FILL_ME_IN_offset))
#define GPIOC_ODR     (*(volatile uint32_t *)(GPIOC_BASE + FILL_ME_IN_offset))

int main(void)
{
    /* ---- STEP B, part 1: clocks -------------------------------------------
     * TODO: turn on the clocks for GPIOB and GPIOC in RCC_AHB1ENR.
     * (Challenge 0 turned on GPIOA's clock with  RCC_AHB1ENR |= (1U << 0); ) */


    /* ---- STEP B, part 2: pin modes ----------------------------------------
     * TODO: make PB5 an output (in GPIOB_MODER) and PC7 an output
     *       (in GPIOC_MODER). Same two lines per pin as Challenge 1:
     *       clear the pin's 2 bits with &= ~(3U << ...), then |= (1U << ...).
     *
     * Hint: PC7's bits start at bit 7*2 = 14.                             */


    while (1) {
        /* ---- STEP C: railroad crossing -------------------------------------
         * TODO:
         *   PB5 on,  PC7 off,  wait 400 ms
         *   PB5 off, PC7 on,   wait 400 ms
         *
         * Hint: each LED is on a different port, so each one uses its own
         *       port's ODR register (GPIOB_ODR vs GPIOC_ODR).
         */

    }
}
