/*
 * =============================================================================
 *  CHALLENGE 9 — SOLUTION   (Dim an LED with PWM)
 * =============================================================================
 *  Steps A-C plus the stretch idea in a comment.
 *
 *  Step A: analogWrite(D12, 25) is about 10 % duty (25 / 255). Change the
 *          number to see 1 (barely on), 128 (about half), 255 (full, the same
 *          as digitalWrite HIGH).
 *
 *  Step B: typical results at 128 (about 50 %). Most people see flicker below
 *          about 50-100 Hz (more if they look sideways, because the edge of
 *          your vision is more sensitive to flicker) and a clear blink below
 *          about 5-10 Hz. At 1 kHz nobody sees anything. Phone slow-motion
 *          video shows flicker at much higher frequencies than eyes do.
 *
 *  Step C: the five levels are 0, 10, 60, 128, 255. Good talking points:
 *            - 0 and 10 look almost the same, and 128 and 255 look almost the
 *              same, even though the gaps in value are 10 and 127. Eyes judge
 *              brightness on a curve, which Challenge 10 corrects.
 *
 *  Stretch: with analogWriteResolution(12), values go 0-4095, so 50 % is about
 *           2048. 12 bits is 4096 brightness steps instead of 256. (The timer
 *           has to count that finely: at 1 kHz it can, since it counts tens
 *           of thousands of ticks per cycle. At very high frequencies it
 *           couldn't.)
 *
 *  THINK answer: one function covers "off", "full on" and everything between,
 *  so code doesn't need special cases. 255 and 0 are PWM's two extremes: the
 *  pin simply stays high or stays low.
 * =============================================================================
 */
#include <Arduino.h>

#define LED_PIN        D12
#define PWM_FREQ_HZ    1000

void setup()
{
    analogWriteFrequency(PWM_FREQ_HZ);

    /* Step A */
    analogWrite(LED_PIN, 25);
}

void loop()
{
    /* Step C: five levels, one second each. (This overrides Step A.) */
    analogWrite(LED_PIN, 0);
    delay(1000);

    analogWrite(LED_PIN, 10);
    delay(1000);

    analogWrite(LED_PIN, 60);
    delay(1000);

    analogWrite(LED_PIN, 128);
    delay(1000);

    analogWrite(LED_PIN, 255);
    delay(1000);
}
