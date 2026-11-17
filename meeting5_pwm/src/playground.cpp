/*
 * =============================================================================
 *  PLAYGROUND — your own PWM experiments
 * =============================================================================
 *  Pick the "env:playground" environment in the VS Code status bar
 *  (or run:  pio run -e playground -t upload ) and build whatever you like.
 *
 *  Ideas: a traffic light that fades between colors, a "heartbeat" with two
 *         pulses, a button (the blue one is USER_BTN) that cycles brightness,
 *         an LED that gets brighter the longer you hold the button.
 *
 *  PWM pins that work here: A0, A1, D12. (D13 and A2 are DAC pins, not PWM.)
 * =============================================================================
 */
#include <Arduino.h>

void setup()
{
    pinMode(USER_BTN, INPUT);
}

void loop()
{
    /* your code here */
    analogWrite(D12, 128);
}
