/*
 * =============================================================================
 *  CHALLENGE 10 — A breathing LED                         (Meeting 5: PWM)
 * =============================================================================
 *
 *  analogWrite()'s value is just a number, so fading is just a loop that
 *  changes it a little bit at a time. In this challenge you'll make an LED
 *  "breathe" and find out why the obvious way looks wrong.
 *
 *  WIRING: the LED on D12 (same as Challenge 9). Step C also needs the LED you
 *  used before on A1:
 *
 *      pin --> LED long leg (+)   LED short leg (-) --> 220-330 ohm --> GND
 *
 *    STEP A:  the fade UP (0 to 255, one step at a time) is done for you. Write
 *             the fade DOWN (255 back to 0) right below it.
 *             DONE WHEN: the LED breathes in and out, about 1 second each way.
 *
 *    STEP B:  does it look even? Watch carefully. It probably seems to leap
 *             up to bright in the first half and then barely change for the
 *             rest. Your eye is not linear: it's very sensitive to dim light
 *             and hardly notices changes in bright light, so doubling the
 *             value doesn't look twice as bright.
 *             The fix: feed the fade through a curve. Fill in curve() so it
 *             returns b CUBED, scaled back to 0-255:
 *                     curve(b) = b * b * b / (255 * 255)
 *             (curve(255) must still be 255, and curve(128) = 32.)
 *             (Both of your fades already call curve(b), so you only fill in
 *             curve() once.)
 *             DONE WHEN: the fade now looks smooth and even all the way.
 *             Hint: b * b * b is at most 16,581,375. That fits in an int
 *             on this chip (32 bits), so no special types are needed.
 *
 *    STEP C:  two LEDs, opposite phase: while D12 fades up, A1 fades down.
 *             DONE WHEN: the two LEDs trade brightness smoothly: when one is
 *             at full, the other is off. (Hint: if D12 is at b, A1 is at
 *             255 - b. Put both through curve().) Add the analogWrite for
 *             LED2_PIN inside both fade loops.
 *
 *    THINK:   we call delay() hundreds of times to make this fade. What else
 *             could the CPU be doing while waiting? (The PWM itself runs in
 *             the timer hardware with no help from the CPU.)
 * =============================================================================
 */
#include <Arduino.h>

#define LED1_PIN   D12
#define LED2_PIN   A1        /* Step C */
#define STEP_MS    4         /* wait between brightness steps: 256 x 4 ms = about 1 s */

/* STEP B: bend the fade to match how eyes see brightness. */
static int curve(int b)
{
    /* TODO: return b cubed, scaled back to 0-255 (see Step B).
     *       For now, just pass the value through.                           */
    return b;
}

void setup()
{
    /* Nothing to set up: analogWrite() prepares the pin the first time you
     * use it. */
}

void loop()
{
    /* Fade UP: b goes 0, 1, 2 ... 255. (Done for you.) */
    for (int b = 0; b <= 255; b++) {
        analogWrite(LED1_PIN, curve(b));
        delay(STEP_MS);
        /* STEP C: TODO: also analogWrite(LED2_PIN, curve(255 - b)); */
    }

    /* STEP A: TODO: fade DOWN. Copy the loop above, but count down:
     *     start b at 255, keep going while b >= 0, and use b-- each time.
     * Don't forget Step C's LED2 line in this loop too.                      */

}
