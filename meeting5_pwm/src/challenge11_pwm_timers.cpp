/*
 * =============================================================================
 *  CHALLENGE 11 — Who owns the frequency?                 (Meeting 5: PWM)
 * =============================================================================
 *
 *  analogWrite() doesn't make the signal itself. A hardware TIMER does:
 *
 *      ONE counter lap time   ->  ONE frequency for the whole timer
 *      several CHANNELS       ->  each channel has its OWN duty cycle
 *
 *  Two pins on the same timer must share a frequency. Two pins on different
 *  timers can run at completely different speeds. In this board:
 *
 *      A0  and  A1   are on the SAME timer (TIM2)
 *      D12           is on a different timer (TIM3)
 *
 *  (The pin table is in README.md in this folder.)
 *
 *  Here's the Arduino catch: analogWriteFrequency(hz) is ONE global setting,
 *  and every analogWrite(pin, value) applies the CURRENT setting to that pin's
 *  timer. So "which frequency does a pin get?" depends on what you called
 *  analogWriteFrequency with just before you last called analogWrite on it
 *  (or on any other pin of the same timer).
 *
 *  We'll use slow frequencies (1 to 10 Hz) so you can SEE each cycle as a
 *  blink. At this speed PWM is just blinking with a duty cycle.
 *
 *  WIRING: LEDs on A0, A1 and D12, the same as before:
 *
 *      pin --> LED long leg (+)   LED short leg (-) --> 220-330 ohm --> GND
 *
 *    STEP A:  two independent rates. Make A0 blink at 1 Hz with 50 % duty
 *             (value 128), and D12 at 4 Hz with 50 % duty. Do it by setting
 *             the frequency, writing that pin, then setting the next one.
 *             DONE WHEN: D12 blinks 4 times for every blink of A0 and they
 *             never drift apart.
 *             Watch out: do all of this in setup(), once. If you call
 *             analogWrite(A0, ...) again in loop(), A0 gets whatever the
 *             frequency is set to at THAT moment.
 *
 *    STEP B:  PREDICT, then test. Add A1 at 10 Hz with a low duty (value 51,
 *             which is 20 %). Before you upload, write in the comment below
 *             what you think A0 will do now. Then upload.
 *             DONE WHEN: you can explain (in that comment) why A0 changed, and
 *             why D12 did not. Hint: which pins share a timer?
 *
 *    STEP C:  same frequency, different duty. Make A0 and A1 both run at 1 Hz,
 *             but A0 at 50 % and A1 at 10 %. Drop Step B's 10 Hz.
 *             DONE WHEN: both start each second together, but A1 gives a short
 *             flash and A0 stays on much longer. (This is the "frequency
 *             belongs to the timer, duty belongs to the channel" idea.)
 *
 *    THINK:   we have three PWM pins here (A0, A1, D12). What's the largest
 *             number of DIFFERENT frequencies you can have running at once?
 *
 *    BONUS:   put an LED on A2 and try analogWrite(A2, 128). A2 is a DAC pin,
 *             not a PWM pin: it outputs a steady voltage. What do you see
 *             compared to the PWM LEDs, and why? (Hint: an LED needs about
 *             2 V before it lights at all.)
 * =============================================================================
 */
#include <Arduino.h>

void setup()
{
    /* STEP A ----------------------------------------------------------------
     * TODO: analogWriteFrequency(1);  analogWrite(A0, 128);
     * TODO: analogWriteFrequency(4);  analogWrite(D12, 128);               */



    /* STEP B ----------------------------------------------------------------
     * My prediction for what happens to A0:
     *
     *
     * TODO: analogWriteFrequency(10);  analogWrite(A1, 51);
     * What actually happened, and why:
     *
     *
     * (When you're done with Step B, delete or comment out its lines.)      */



    /* STEP C ----------------------------------------------------------------
     * TODO: A0 and A1 both at 1 Hz; A0 at 128 (50 %), A1 at 26 (10 %).      */


}

void loop()
{
    /* Nothing to do here. The timers keep the LEDs going on their own.
     * That's the point of hardware PWM: the CPU is free.                    */
}
