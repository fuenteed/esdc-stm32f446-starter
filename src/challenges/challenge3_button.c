/*
 * =============================================================================
 *  CHALLENGE 3 — Add input: the blue USER button   (skill: reading IDR)
 * =============================================================================
 *
 *  WIRING: same 4 LEDs as Challenge 1 (A0, A1, A2, D12). The button is the
 *          BLUE button on the Nucleo (B1, "USER"). It's on pin PC13. No
 *          extra wiring needed.
 *
 *  IMPORTANT: the button reads 0 when PRESSED and 1 when released.
 *  (It's wired with a pull-up resistor: released = pulled up to 3.3 V = 1,
 *   pressed = connected to ground = 0.)  This is called "active low".
 *
 *  The chase from Challenge 1 is already running in this file.
 *
 *    STEP A:  in setup(), make PC13 an input. Then write button_is_pressed()
 *             and use it to light the onboard green LED (PA5) while the
 *             button is held down.
 *             DONE WHEN: green LED on while held, off when released.
 *
 *    STEP B:  pause the chase while the button is held.
 *             DONE WHEN: holding the button freezes the light.
 *
 *    STEP C:  each PRESS reverses the chase direction (not each 10 ms that
 *             the button happens to be down!)
 *             DONE WHEN: one press = one reversal, every time.
 *
 *    STRETCH — "Stop the light" game: chase runs; press when the light is
 *             on LED 2. Hit = flash all LEDs and speed up. Miss = start over.
 *             Challenge a friend.
 * =============================================================================
 */
#include "esdc.h"

/* GPIOC registers (these are the Challenge 2 answers for Port C — compare!) */
#define GPIOC_BASE    0x40020800U
#define GPIOC_MODER   (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_IDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x10U))

#define BUTTON_PIN    13U     /* PC13 */

static const uint32_t LED_PINS[4] = { 0, 1, 4, 6 };

static void setup(void)
{
    RCC_AHB1ENR |= (1U << 0);       /* clock on: GPIOA (LEDs)    */
    RCC_AHB1ENR |= (1U << 2);       /* clock on: GPIOC (button)  */

    /* LEDs + onboard green LED (PA5) as outputs: the Challenge 1 answer. */
    for (int i = 0; i < 4; i++) {
        GPIOA_MODER &= ~(3U << (LED_PINS[i] * 2U));
        GPIOA_MODER |=  (1U << (LED_PINS[i] * 2U));
    }
    GPIOA_MODER &= ~(3U << (5U * 2U));
    GPIOA_MODER |=  (1U << (5U * 2U));

    /* ---- STEP A, part 1 ----------------------------------------------------
     * TODO: make PC13 (BUTTON_PIN) an input in GPIOC_MODER.
     *
     * Hint 1: input is 00. Which ONE of the two output lines above do you
     *         still need?
     * Hint 2: pins come out of reset as inputs, so this works even if you
     *         skip it. Write it anyway: never rely on a default you
     *         didn't check.                                                 */

}

/* ---------------------------------------------------------------------------
 *  STEP A, part 2 — read the button
 *  Return 1 if the button is pressed right now, 0 if not.
 * ------------------------------------------------------------------------ */
static int button_is_pressed(void)
{
    /* TODO: read bit 13 of GPIOC_IDR.
     *
     * Hint 1: TEST a bit with  (REG & (1U << n))  — it's nonzero if the bit is 1.
     * Hint 2: the button is ACTIVE LOW. Pressed means the bit is 0.
     * Hint 3: return ((GPIOC_IDR & (1U << BUTTON_PIN)) == 0U); ... try to
     *         explain to your partner why == 0 is there before using it!
     */
    return 0;
}

/* Turn off the current LED, move one step in `direction` (+1 or -1),
 * wrap around at the ends, and turn on the new LED. (Done for you.) */
static int chase_step(int position, int direction)
{
    GPIOA_ODR &= ~(1U << LED_PINS[position]);

    position += direction;
    if (position > 3) { position = 0; }
    if (position < 0) { position = 3; }

    GPIOA_ODR |= (1U << LED_PINS[position]);
    return position;
}

int main(void)
{
    setup();

    int position  = 0;      /* which LED is lit (0-3)                  */
    int direction = +1;     /* +1 = forward, -1 = backward             */
    int ticks     = 0;      /* counts 10 ms ticks                      */
    int was_pressed = 0;    /* button state on the PREVIOUS tick (Step C) */

    GPIOA_ODR |= (1U << LED_PINS[position]);

    /* WHY 10 ms TICKS INSTEAD OF delay_ms(150)?
     * During delay_ms() the chip does nothing else, so a quick button press
     * would be missed. Instead we wake up every 10 ms, check the button,
     * and only move the chase every 15 ticks (15 x 10 ms = 150 ms).     */
    while (1) {
        delay_ms(10);
        ticks++;

        int pressed = button_is_pressed();

        /* ---- STEP A ----------------------------------------------------
         * TODO: if pressed, turn PA5 on. Otherwise turn PA5 off.          */


        /* ---- STEP C ----------------------------------------------------
         * TODO: if the button is pressed NOW but was NOT pressed last tick,
         *       that's a new press: flip the direction (direction = -direction).
         *
         * This "was it different last time?" check is called EDGE DETECTION. */


        was_pressed = pressed;
        (void)was_pressed;   /* silences a warning until you use it in Step C */

        /* ---- STEP B ----------------------------------------------------
         * TODO: only move the chase when the button is NOT held.
         *       (Wrap the if-statement below in another check.)            */
        if (ticks >= 15) {
            ticks = 0;
            position = chase_step(position, direction);
        }
    }
}
