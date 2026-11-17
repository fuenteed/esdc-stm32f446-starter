/*
 * =============================================================================
 *  CHALLENGE 12 — SOLUTION   (A scanner with a fading tail)
 * =============================================================================
 *  Steps A-C. Set SMOOTH to 0 for the jumping version (Steps A + B).
 *
 *  Step A: PATH = { 0, 1, 2, 1 }. The head visits LED 0, 1, 2, then back to 1,
 *          then starts over at 0, so it bounces.
 *  Step B: BY_DISTANCE = { 255, 40, 6 }. At the default 1000 Hz, 40 and 6 are
 *          clearly different, and the 6 LED still shows as a faint glow.
 *
 *  Step C: LED i sits at position i * 1000. An LED's level falls off in a
 *  straight line with its distance from `position`, reaching 0 when the
 *  light is a full LED-spacing away. Then the cube curve (Challenge 10)
 *  makes the tail look natural. Because the LEDs are 1000 units apart, at
 *  any moment at most two neighbours are lit: the light seems to slide.
 * =============================================================================
 */
#include <Arduino.h>

#define NUM_LEDS   3
#define STEP_MS    150          /* Steps A + B */
#define SMOOTH     1            /* 1 = Step C (slides), 0 = Steps A + B (jumps) */

static const int LEDS[NUM_LEDS] = { A0, A1, D12 };

/* Step B */
static const int BY_DISTANCE[NUM_LEDS] = { 255, 40, 6 };

/* Step A */
static const int PATH[4] = { 0, 1, 2, 1 };
static int step = 0;

/* Step C */
static int position  = 0;       /* 0 to 2000 */
static int direction = 1;       /* +1 = moving right, -1 = moving left */

/* Cube curve on 0-1000: x * x * x / 1000 / 1000, written to stay in 32 bits. */
static int curve1000(int x)
{
    return x * x / 1000 * x / 1000;
}

/* Steps A + B: one jump of the scanner. */
static void jump_once()
{
    int head = PATH[step];

    for (int i = 0; i < NUM_LEDS; i++) {
        int distance = i - head;
        if (distance < 0) {
            distance = -distance;
        }
        analogWrite(LEDS[i], BY_DISTANCE[distance]);
    }
    delay(STEP_MS);

    step = step + 1;
    if (step == 4) {
        step = 0;
    }
}

/* Step C: one tiny slide of the scanner. */
static void slide_once()
{
    for (int i = 0; i < NUM_LEDS; i++) {
        int distance = position - i * 1000;
        if (distance < 0) {
            distance = -distance;
        }
        int level = 1000 - distance;
        if (level < 0) {
            level = 0;
        }
        analogWrite(LEDS[i], curve1000(level) * 255 / 1000);
    }
    delay(5);

    position = position + direction * 10;
    if (position >= 2000 || position <= 0) {
        direction = -direction;
    }
}

void setup()
{
}

void loop()
{
    if (SMOOTH) {
        slide_once();
    } else {
        jump_once();
    }
}
