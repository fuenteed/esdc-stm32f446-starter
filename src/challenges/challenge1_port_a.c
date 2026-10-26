/*
 * =============================================================================
 *  CHALLENGE 1 — Blink several pins on Port A   (skill: bit masking)
 * =============================================================================
 *
 *  WIRING (4 LEDs on the breadboard):
 *
 *      Nucleo header pin  ->  LED long leg (+)  ...  LED short leg (-)
 *                                                       -> 220-330 Ω resistor -> GND
 *
 *      LED 0:  A0  (= PA0)
 *      LED 1:  A1  (= PA1)
 *      LED 2:  A2  (= PA4)    <- yes, A2 is PA4, not PA2
 *      LED 3:  D12 (= PA6)
 *
 *  The pin setup (clock + output mode) is already done for you below.
 *  Your job is only the ODR register: which bits are 1 and which are 0.
 *
 *  There are three steps. Do them in order: write the code in a step's
 *  function, then change the line in main() to call that step.
 *
 *    STEP A:  turn all 4 LEDs on (and leave them on)
 *             DONE WHEN: all 4 LEDs light up. (If one doesn't, it's wiring!)
 *
 *    STEP B:  blink all 4 together, on 500 ms, off 500 ms
 *             DONE WHEN: all 4 blink in sync.
 *
 *    STEP C:  "chase": light one LED at a time, LED 0 -> 1 -> 2 -> 3, repeat
 *             DONE WHEN: a single light runs across your LEDs.
 *
 *    STRETCH: make the chase bounce back and forth (Knight Rider), or count
 *             0 to 15 in binary on the 4 LEDs.
 *
 *  Cheat sheet is at the top of include/esdc.h.
 * =============================================================================
 */
#include "esdc.h"

/* Which bit (pin number) each LED is on. LED_PINS[2] is 4 because LED 2 is PA4.
 * Using this array lets you loop over the LEDs: LED_PINS[i]                 */
static const uint32_t LED_PINS[4] = { 0, 1, 4, 6 };

static void setup(void)
{
    RCC_AHB1ENR |= (1U << 0);           /* clock on for GPIOA */

    for (int i = 0; i < 4; i++) {
        PIN_OUTPUT(GPIOA_MODER, LED_PINS[i]);
    }
    PIN_OUTPUT(GPIOA_MODER, 5);         /* onboard green LED too */
}

/* ---------------------------------------------------------------------------
 *  STEP A — turn all 4 LEDs on
 * ------------------------------------------------------------------------ */
static void step_a_all_on(void)
{
    /* Example: this turns on the onboard LED (PA5).                       */
    GPIOA_ODR |= (1U << 5);

    /* TODO: turn on PA0, PA1, PA4 and PA6 the same way.
     *
     * Hint 1: one line per pin works fine.
     * Hint 2: or do all of them in one line by OR-ing masks together:
     *         GPIOA_ODR |= (1U << a) | (1U << b) | ...;                   */


    while (1) {
        /* nothing else to do: stay here with the LEDs on */
    }
}

/* ---------------------------------------------------------------------------
 *  STEP B — blink all 4 together
 * ------------------------------------------------------------------------ */
static void step_b_blink_together(void)
{
    while (1) {
        /* TODO: flip all 4 LEDs, then wait 500 ms.
         *
         * Hint 1: which operator TOGGLES a bit? (see the cheat sheet)
         * Hint 2: toggle every bit in one statement, then call delay_ms(500).
         */

    }
}

/* ---------------------------------------------------------------------------
 *  STEP C — chase
 * ------------------------------------------------------------------------ */
static void step_c_chase(void)
{
    while (1) {
        for (int i = 0; i < 4; i++) {
            /* TODO:
             *   1. turn ON the LED on pin LED_PINS[i]
             *   2. wait a bit (try 150 ms)
             *   3. turn it OFF
             *
             * Hint: (1U << LED_PINS[i]) is the mask for LED number i.
             *       SET uses |=, CLEAR uses &= ~(...)
             */

        }
    }
}

int main(void)
{
    setup();

    /* Change this line to step_b_blink_together(); or step_c_chase();
     * when you finish each step. */
    step_a_all_on();

    /* (never reached: each step loops forever) */
    (void)step_b_blink_together;
    (void)step_c_chase;
    while (1) { }
}
