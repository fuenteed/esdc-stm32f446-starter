/*
 * =============================================================================
 *  CHALLENGE 9 — Dim an LED with PWM                      (Meeting 5: PWM)
 * =============================================================================
 *
 *  This meeting is different: we use the ARDUINO framework. No registers.
 *  You'll use the same functions millions of Arduino users do:
 *
 *      analogWrite(pin, value);        // value 0 to 255 -> that much brightness
 *
 *  So far every pin has been ON or OFF. A pin can't output "half a volt", so
 *  analogWrite() cheats: it flips the pin on and off very fast using a
 *  hardware timer, and your eye sees the AVERAGE. That's PWM
 *  (pulse width modulation).
 *
 *      value  25  ┌┐        ┌┐        ┌┐        looks dim
 *                 ┘└────────┘└────────┘└──
 *      value 128  ┌───┐     ┌───┐     ┌───┐     looks medium
 *                 ┘   └─────┘   └─────┘   └─
 *      value 230  ┌────────┐┌────────┐┌───      looks bright
 *                 ┘        └┘        └┘
 *
 *      FREQUENCY  = how many on/off cycles per second (Hz)
 *      DUTY CYCLE = the share of each cycle the pin is ON.
 *                   analogWrite value 255 = 100 %, 128 = about 50 %, 0 = 0 %
 *
 *  WIRING: one LED on D12, the same wiring as in earlier meetings:
 *
 *      D12 --> LED long leg (+)   LED short leg (-) --> 220-330 ohm --> GND
 *
 *  Not every pin can do PWM, and the onboard green LED (D13) can't: on this
 *  board D13 and A2 are wired to a DAC, so analogWrite() gives them a steady
 *  voltage instead of PWM. The pins that work for PWM here are A0, A1 and D12.
 *  See README.md in this folder for the pin table.
 *
 *    STEP A:  make the LED dim: 10 % duty (about 25 out of 255). Then try
 *             128, 1, and 255.
 *             DONE WHEN: you can see the LED at clearly different brightnesses
 *             just by changing one number.
 *
 *    STEP B:  FIND THE FLICKER. Keep the value at 128 (about 50 %) and lower
 *             PWM_FREQ_HZ: 1000, 200, 100, 50, 20, 10, 5, 2, 1. Re-upload each
 *             time.
 *             DONE WHEN: you've written down (in a comment!) the frequency
 *             where YOU first see the LED flicker, and the one where it
 *             clearly blinks. Compare with your partner. Same number?
 *             Hint: slow-motion video on a phone shows flicker your eye
 *             misses.
 *
 *    STEP C:  a brightness staircase. In loop(), show 5 different brightness
 *             values, each for 1 second, e.g. 0, 10, 60, 128, 255. loop() runs
 *             again and again, so the staircase repeats forever.
 *             DONE WHEN: you see 5 distinct steps. Which two steps look the
 *             most alike? Which look the most different?
 *
 *    STRETCH: add  analogWriteResolution(12);  at the top of setup(). Now
 *             analogWrite takes 0 to 4095. What value is 50 % now? How many
 *             different brightness steps do you have compared to before?
 *
 *    THINK:   255 is the same as plain digitalWrite(pin, HIGH), and 0 is the
 *             same as LOW. So why is it useful that PWM covers the whole range
 *             from 0 to 255 with one function?
 * =============================================================================
 */
#include <Arduino.h>

#define LED_PIN        D12

/* STEP B: change this number and re-upload. */
#define PWM_FREQ_HZ    1000

void setup()
{
    /* Done for you: how many on/off cycles per second analogWrite() uses.
     * (Call it BEFORE analogWrite. 1000 is the default.)                    */
    analogWriteFrequency(PWM_FREQ_HZ);

    /* STEP A: TODO: set the brightness to about 10 %.
     *   Hint: analogWrite(LED_PIN, ...);
     * Then try other numbers.                                               */

}

void loop()
{
    /* STEP C: TODO: show five brightness levels, one second each.
     * Write this pair of lines 5 times, with a different number each time:
     *
     *     analogWrite(LED_PIN, 0);
     *     delay(1000);
     *
     * (delay(1000) means "wait 1000 ms". Once this works it overrides the
     * single value you set in Step A.)                                      */

}
