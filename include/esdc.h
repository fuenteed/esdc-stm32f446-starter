/*
 * =============================================================================
 *  esdc.h  —  Include this at the top of every challenge file.
 * =============================================================================
 *
 *  It gives you:
 *    - the register names from esdc_registers.h (RCC_AHB1ENR, GPIOA_ODR, ...)
 *    - delay_ms()           wait a number of milliseconds
 *    - PIN_OUTPUT() / PIN_INPUT()   used by Challenge 0 only. From
 *                                   Challenge 1 on you set pin modes
 *                                   yourself with shifts (see below).
 *
 *  ---------------------------------------------------------------------------
 *  BIT MASKING CHEAT SHEET  (REG = any register, n = bit number, from 0)
 *
 *      REG |=  (1U << n);          // SET bit n to 1, leave all others alone
 *      REG &= ~(1U << n);          // CLEAR bit n to 0, leave all others alone
 *      REG ^=  (1U << n);          // TOGGLE bit n (0->1, 1->0)
 *      if (REG & (1U << n)) {...}  // TEST: true if bit n is 1
 *
 *      (1U << n) means "a 1 shifted left n places" = only bit n is 1.
 *          1U << 0  = 0b0000_0001
 *          1U << 5  = 0b0010_0000  (= 32 in decimal)
 *      The U means "unsigned". Always use it with registers.
 *
 *      Set several bits at once:   REG |= (1U << 0) | (1U << 4);
 *
 *  ---------------------------------------------------------------------------
 *  2-BIT FIELDS: setting a pin's mode in MODER
 *
 *      MODER gives every pin 2 bits:  00 = input   01 = output
 *      Pin n's 2 bits start at bit 2*n.  PA5 -> bits 11:10.
 *
 *      GPIOA_MODER &= ~(3U << (5U * 2U));  // CLEAR both bits of PA5 -> 00
 *      GPIOA_MODER |=  (1U << (5U * 2U));  // then SET the low one   -> 01
 *
 *      3U is 0b11, a mask covering both bits. Always clear first: if the
 *      bits were 10 or 11, only OR-ing in 01 would leave them wrong.
 *      For INPUT the clear alone is enough (00).
 *
 *  !! Do NOT write  REG = (1U << 5);  — the plain = wipes every other bit
 *     to 0, which turns off every other pin on that port.
 * =============================================================================
 */
#ifndef ESDC_H
#define ESDC_H

#include <stdint.h>
#include "esdc_registers.h"

/* The chip runs from its internal 16 MHz oscillator (HSI) after reset.
 * We never change it, so every calculation in this repo assumes 16 MHz.   */
#define ESDC_CPU_HZ     16000000UL

/* Wait about `ms` milliseconds by spinning in a loop (see
 * src/esdc_helpers.c if you're curious). Nothing else runs while you wait,
 * so a button press during a delay is missed.                              */
void delay_ms(uint32_t ms);

/* Set pin `pin` to OUTPUT or INPUT mode in a MODER register.
 *
 *   PIN_OUTPUT(GPIOA_MODER, 5);   // PA5 is now an output
 *
 * Challenge 0 uses these so you can test your setup before learning
 * anything. They do exactly the two lines in "2-BIT FIELDS" above.
 * From Challenge 1 on, write those lines yourself instead.                 */
#define PIN_OUTPUT(moder_reg, pin) \
    ((moder_reg) = ((moder_reg) & ~(3U << ((pin) * 2U))) | (1U << ((pin) * 2U)))

#define PIN_INPUT(moder_reg, pin) \
    ((moder_reg) &= ~(3U << ((pin) * 2U)))

#endif /* ESDC_H */
