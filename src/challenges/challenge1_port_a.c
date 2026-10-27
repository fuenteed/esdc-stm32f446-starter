/*
 * =============================================================================
 *  CHALLENGE 1 — SOLUTION
 * =============================================================================
 *  Wiring: LEDs on A0 (PA0), A1 (PA1), A2 (PA4), D12 (PA6), each through a
 *  220-330 Ω resistor to GND.
 *
 *  Change the call in main() to try each step, including the two stretch
 *  goals at the bottom (bounce and binary counter).
 * =============================================================================
 */
#include "esdc.h"

static const uint32_t LED_PINS[4] = { 0, 1, 4, 6 };

/* All four LED bits in one mask: 0b0101_0011 */
#define ALL_LEDS  ((1U << 0) | (1U << 1) | (1U << 4) | (1U << 6))

static void setup(void)
{
    RCC_AHB1ENR |= (1U << 0);

    /* Each pin: clear its 2 MODER bits, then write 01 (output). */
    for (int i = 0; i < 4; i++) {
        GPIOA_MODER &= ~(3U << (LED_PINS[i] * 2U));
        GPIOA_MODER |=  (1U << (LED_PINS[i] * 2U));
    }
    GPIOA_MODER &= ~(3U << (5U * 2U));
    GPIOA_MODER |=  (1U << (5U * 2U));
}

static void step_a_all_on(void)
{
    GPIOA_ODR |= (1U << 5);
    GPIOA_ODR |= (1U << 0) | (1U << 1) | (1U << 4) | (1U << 6);

    while (1) { }
}

static void step_b_blink_together(void)
{
    while (1) {
        GPIOA_ODR ^= ALL_LEDS;
        delay_ms(500);
    }
}

static void step_c_chase(void)
{
    while (1) {
        for (int i = 0; i < 4; i++) {
            GPIOA_ODR |=  (1U << LED_PINS[i]);
            delay_ms(150);
            GPIOA_ODR &= ~(1U << LED_PINS[i]);
        }
    }
}

/* STRETCH 1: bounce 0 1 2 3 2 1 0 1 2 ... */
static void stretch_bounce(void)
{
    int i = 0;
    int dir = +1;

    while (1) {
        GPIOA_ODR |=  (1U << LED_PINS[i]);
        delay_ms(120);
        GPIOA_ODR &= ~(1U << LED_PINS[i]);

        if (i == 3) { dir = -1; }
        if (i == 0) { dir = +1; }
        i += dir;
    }
}

/* STRETCH 2: count 0..15 in binary. LED 0 is the lowest bit.
 * The pins aren't next to each other (0, 1, 4, 6), so we can't just
 * write `count` into ODR. Instead, check each bit of count and copy it
 * to the matching pin. */
static void stretch_binary_counter(void)
{
    uint32_t count = 0;

    while (1) {
        for (int bit = 0; bit < 4; bit++) {
            if (count & (1U << bit)) {
                GPIOA_ODR |=  (1U << LED_PINS[bit]);
            } else {
                GPIOA_ODR &= ~(1U << LED_PINS[bit]);
            }
        }
        delay_ms(500);
        count = (count + 1U) % 16U;
    }
}

int main(void)
{
    setup();

    step_c_chase();

    (void)step_a_all_on;
    (void)step_b_blink_together;
    (void)stretch_bounce;
    (void)stretch_binary_counter;
    while (1) { }
}
