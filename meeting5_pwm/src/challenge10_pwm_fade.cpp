/*
 * =============================================================================
 *  CHALLENGE 10 — SOLUTION   (A breathing LED)
 * =============================================================================
 *  Steps A-C together: D12 fades up while A1 fades down, then they swap.
 *  Both go through the cubed curve from Step B.
 *
 *  Why cubed? Perceived brightness grows roughly like (light power)^(1/3)
 *  for a dim LED in a normal room, so to make brightness LOOK linear, the
 *  value has to grow like b^3. (Real displays use a "gamma" of about 2.2 for
 *  the same reason. A cube is easy to compute and close enough.)
 *
 *  To see only Step A (no curve), change curve() to "return b;".
 *  To see only D12 (no second LED), delete the LED2_PIN lines.
 *
 *  THINK answer: it could read a button, run a sensor, talk over Serial,
 *  anything. delay() blocks all of that, which is why Meeting 4's idea of
 *  checking the clock instead of waiting (millis() in Arduino) matters.
 * =============================================================================
 */
#include <Arduino.h>

#define LED1_PIN   D12
#define LED2_PIN   A1
#define STEP_MS    4

/* Step B: b cubed, scaled back to 0-255. 255 * 255 * 255 = 16,581,375 fits
 * in a 32-bit int. */
static int curve(int b)
{
    return (b * b * b) / (255 * 255);
}

void setup()
{
}

void loop()
{
    /* Fade LED1 up (and LED2 down). */
    for (int b = 0; b <= 255; b++) {
        analogWrite(LED1_PIN, curve(b));
        analogWrite(LED2_PIN, curve(255 - b));
        delay(STEP_MS);
    }

    /* Fade LED1 down (and LED2 up). */
    for (int b = 255; b >= 0; b--) {
        analogWrite(LED1_PIN, curve(b));
        analogWrite(LED2_PIN, curve(255 - b));
        delay(STEP_MS);
    }
}
