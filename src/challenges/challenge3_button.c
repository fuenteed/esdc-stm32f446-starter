/*
 * =============================================================================
 *  CHALLENGE 3 — SOLUTION
 * =============================================================================
 *  Wiring: same 4 LEDs as Challenge 1. Button = blue USER button (PC13).
 *
 *  main() does Steps A, B and C.  For the "Stop the light" stretch game,
 *  change main() to call stop_the_light_game() instead.
 * =============================================================================
 */
#include "esdc.h"

#define GPIOC_BASE    0x40020800U
#define GPIOC_MODER   (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_IDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x10U))

#define BUTTON_PIN    13U

static const uint32_t LED_PINS[4] = { 0, 1, 4, 6 };
#define ALL_LEDS  ((1U << 0) | (1U << 1) | (1U << 4) | (1U << 6))

static void setup(void)
{
    RCC_AHB1ENR |= (1U << 0);
    RCC_AHB1ENR |= (1U << 2);

    for (int i = 0; i < 4; i++) {
        PIN_OUTPUT(GPIOA_MODER, LED_PINS[i]);
    }
    PIN_OUTPUT(GPIOA_MODER, 5);
    PIN_INPUT(GPIOC_MODER, BUTTON_PIN);
}

/* Active low: the bit is 0 while the button is held. */
static int button_is_pressed(void)
{
    return ((GPIOC_IDR & (1U << BUTTON_PIN)) == 0U);
}

static int chase_step(int position, int direction)
{
    GPIOA_ODR &= ~(1U << LED_PINS[position]);

    position += direction;
    if (position > 3) { position = 0; }
    if (position < 0) { position = 3; }

    GPIOA_ODR |= (1U << LED_PINS[position]);
    return position;
}

/* ---------------------------------------------------------------------------
 *  STRETCH: Stop the light
 *  The light bounces across the 4 LEDs. Press when it's on LED 2 (PA4).
 *  Hit  -> all LEDs flash, the game gets faster.
 *  Miss -> onboard LED flashes, speed resets.
 * ------------------------------------------------------------------------ */
#define TARGET_LED  2

static void flash(uint32_t mask, int times)
{
    for (int i = 0; i < times; i++) {
        GPIOA_ODR |= mask;   delay_ms(80);
        GPIOA_ODR &= ~mask;  delay_ms(80);
    }
}

static void stop_the_light_game(void)
{
    int position = 0;
    int direction = +1;
    int ticks = 0;
    int ticks_per_step = 20;        /* 20 x 10 ms = 200 ms per step to start */
    int was_pressed = 0;

    GPIOA_ODR |= (1U << LED_PINS[position]);

    while (1) {
        delay_ms(10);
        ticks++;

        int pressed = button_is_pressed();

        if (pressed && !was_pressed) {               /* new press */
            GPIOA_ODR &= ~ALL_LEDS;
            if (position == TARGET_LED) {
                flash(ALL_LEDS, 3);
                if (ticks_per_step > 4) { ticks_per_step -= 3; }   /* faster */
            } else {
                flash(1U << 5, 3);
                ticks_per_step = 20;                                /* reset  */
            }
            position = 0;
            direction = +1;
            ticks = 0;
            GPIOA_ODR |= (1U << LED_PINS[position]);
        }
        was_pressed = pressed;

        if (ticks >= ticks_per_step) {
            ticks = 0;
            if (position == 3) { direction = -1; }
            if (position == 0) { direction = +1; }
            position = chase_step(position, direction);
        }
    }
}

int main(void)
{
    setup();

    int position  = 0;
    int direction = +1;
    int ticks     = 0;
    int was_pressed = 0;

    GPIOA_ODR |= (1U << LED_PINS[position]);

    (void)stop_the_light_game;   /* swap this in for the stretch game */

    while (1) {
        delay_ms(10);
        ticks++;

        int pressed = button_is_pressed();

        /* Step A: mirror the button on the onboard LED */
        if (pressed) {
            GPIOA_ODR |=  (1U << 5);
        } else {
            GPIOA_ODR &= ~(1U << 5);
        }

        /* Step C: edge detection -> one press, one reversal */
        if (pressed && !was_pressed) {
            direction = -direction;
        }
        was_pressed = pressed;

        /* Step B: only advance while the button is NOT held */
        if (!pressed) {
            if (ticks >= 15) {
                ticks = 0;
                position = chase_step(position, direction);
            }
        }
    }
}
