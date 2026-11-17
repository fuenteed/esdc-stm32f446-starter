/*
 * =============================================================================
 *  CHALLENGE 12 (STRETCH) — A scanner with a fading tail   (Meeting 5: PWM)
 * =============================================================================
 *
 *  A light bounces back and forth along a row of LEDs, leaving a glowing
 *  trail behind it. Without PWM, an LED is on or off, so you can't make a
 *  tail. With analogWrite(), "a little bit on" is easy.
 *
 *      head moves:   LED0  LED1  LED2  LED1  LED0 ...
 *      brightness:   LED at the head = bright, next to it = dim, further = off
 *
 *  WIRING: three LEDs in a row on A0, A1 and D12, the same as before. They are
 *  LED number 0, 1 and 2 in the LEDS list below. (A2 isn't used: it can't do
 *  PWM.)
 *
 *  HOW IT WORKS (done for you): the light has a position called `head`: 0, 1
 *  or 2. loop() lights every LED according to how far it is from the head,
 *  using the BY_DISTANCE table, then waits, then moves on.
 *
 *    STEP A:  make the head MOVE back and forth. PATH lists where the head goes,
 *             one entry per move: 0, 1, 2, 1, then it starts over. Fill in
 *             PATH, then write the code at the bottom of loop() that moves to
 *             the next entry.
 *             DONE WHEN: one light sweeps back and forth.
 *
 *    STEP B:  add the tail. BY_DISTANCE[0] is the value for the LED AT the head,
 *             [1] for an LED one step away, [2] for two steps away. Right now
 *             only the head is lit. Make the head 255, the LED next to it
 *             something dim like 40, and the one two away 6 or even 0.
 *             DONE WHEN: the sweep leaves a glowing trail. Remember Challenge
 *             10: your eye wants the dim levels MUCH dimmer than you'd think.
 *
 *    STEP C (hard):  smooth motion. Right now the light jumps from LED to LED.
 *             Instead, let `position` be a number from 0 to 2000 where LED i
 *             sits at i * 1000. Each LED's brightness falls off in a straight
 *             line with how far it is from the position:
 *                     level = 1000 - |position - i * 1000|,  but never below 0
 *             That gives 0 to 1000. Turn it into an analogWrite value
 *             (0 to 255) AFTER passing it through a cube curve, like Challenge
 *             10 (cube in 0-1000: level * level / 1000 * level / 1000).
 *             Move `position` by 10 every 5 ms and bounce at 0 and 2000.
 *             DONE WHEN: the light SLIDES between LEDs, with no visible jump.
 *
 *    STRETCH ideas: make it faster or slower with STEP_MS, or add a second
 *             light chasing the first.
 *
 *    A NOTE ON TIMERS: A0 and A1 are on the same timer and D12 is on another,
 *    but the frequency is the default 1000 Hz for all of them, so nothing
 *    odd happens. (See Challenge 11 for what happens when you change it.)
 * =============================================================================
 */
#include <Arduino.h>

#define NUM_LEDS   3
#define STEP_MS    150          /* time between moves in Steps A and B */

static const int LEDS[NUM_LEDS] = { A0, A1, D12 };

/* STEP B: the analogWrite value (0-255) for an LED, by how far it is from the head.
 *   [0] = the head itself   [1] = one LED away   [2] = two LEDs away       */
static const int BY_DISTANCE[NUM_LEDS] = { 255, 0, 0 };   /* TODO */

/* STEP A: where the head goes, one entry per move. After the last entry it
 * starts over from the first.                                               */
static const int PATH[4] = { 0, 0, 0, 0 };                /* TODO */

static int step = 0;            /* which entry of PATH we are on (0 to 3) */

void setup()
{
    /* Nothing to set up. */
}

void loop()
{
    int head = PATH[step];      /* where the light is right now */

    /* Light every LED. (Done for you.) For each LED, work out how many steps
     * it is from the head, then look up its value in BY_DISTANCE.           */
    for (int i = 0; i < NUM_LEDS; i++) {
        int distance = i - head;
        if (distance < 0) {
            distance = -distance;       /* make it positive */
        }
        analogWrite(LEDS[i], BY_DISTANCE[distance]);
    }

    delay(STEP_MS);

    /* STEP A: TODO: go to the next entry of PATH:
     *     step = step + 1;
     * and when step reaches 4, set it back to 0 (use an if).                */

}
