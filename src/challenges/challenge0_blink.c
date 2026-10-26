/*
 * =============================================================================
 *  CHALLENGE 0 — Check your setup
 * =============================================================================
 *  Nothing to write here. Build and upload this first.
 *
 *  DONE WHEN: the green LED on the board (LD2, next to the white button area)
 *             blinks about twice per second.
 *
 *  If it blinks, your toolchain, USB cable and board all work. Move on to
 *  Challenge 1. If not, see "Troubleshooting" in the README.
 * =============================================================================
 */
#include "esdc.h"

int main(void)
{
    /* Step zero: turn on the clock for GPIO port A (bit 0 of RCC_AHB1ENR).
     * Without this, every write to GPIOA is ignored. */
    RCC_AHB1ENR |= (1U << 0);

    /* Make PA5 an output. PA5 is wired to the green LED on the Nucleo. */
    PIN_OUTPUT(GPIOA_MODER, 5);

    /* Embedded programs never end: main() loops forever. */
    while (1) {
        GPIOA_ODR ^= (1U << 5);   /* toggle bit 5 -> LED flips on/off */
        delay_ms(250);
    }
}
