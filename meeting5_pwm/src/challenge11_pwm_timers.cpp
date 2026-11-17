/*
 * =============================================================================
 *  CHALLENGE 11 — SOLUTION   (Who owns the frequency?)
 * =============================================================================
 *  Steps A + C, plus the THINK answer in a comment.
 *
 *  Step B (the prediction): analogWriteFrequency(10); analogWrite(A1, 51)
 *  sets the frequency of TIM2, and A0 is on TIM2 too, so A0 jumps from 1 Hz
 *  to 10 Hz. D12 is on TIM3, a different timer, so it keeps blinking at 4 Hz.
 *  Rule: one frequency per TIMER, one duty per CHANNEL.
 *  (A0's on-time may look different too. The core changes the timer's period
 *  but does not rescale the other channel's compare value, so if you ever see
 *  A0's brightness shift after changing the frequency, rewrite its value with
 *  analogWrite after the change.)
 *
 *  THINK answer: 2 with these three pins: TIM2 (A0 + A1) and TIM3 (D12).
 *  More PWM pins on other timers (for example D10 is on TIM4) would allow more.
 *
 *  BONUS answer: A2 (PA4) is wired to the chip's DAC, so analogWrite(A2, 128)
 *  outputs a steady voltage (about half of 3.3 V) instead of PWM. The LED
 *  looks dark or dim, with no flicker at any speed: an LED needs roughly
 *  2 V to start glowing, so half the supply is barely enough, and below
 *  that nothing lights. PWM LEDs don't have this problem because the
 *  pin is always either 0 V or the full 3.3 V.
 * =============================================================================
 */
#include <Arduino.h>

void setup()
{
    /* Step A: A0 at 1 Hz, D12 at 4 Hz, 50 % each. */
    analogWriteFrequency(1);
    analogWrite(A0, 128);

    analogWriteFrequency(4);
    analogWrite(D12, 128);

    /* Step C: A1 joins A0's timer at the SAME frequency (1 Hz) with a
     * different duty. The frequency must be 1 again when we write A1,
     * otherwise TIM2 (and A0 with it) would change speed.                   */
    analogWriteFrequency(1);
    analogWrite(A1, 26);
    analogWrite(A0, 128);      /* rewrite A0 so its duty is for this period too */
}

void loop()
{
}
