/*
 * =============================================================================
 *  CHALLENGE 7 (STRETCH) — Four blink rates at once    (Meeting 4: Timers)
 * =============================================================================
 *
 *  Try this with delay_ms(): blink one LED every 100 ms and another every
 *  1000 ms. You can't. While the code waits 1000 ms, nothing else happens.
 *
 *  The trick: NEVER wait. Let TIM1 run as a clock that never gets reset
 *  (like a wall clock), and keep asking "has it been long enough since I
 *  last toggled this LED?"
 *
 *      now = millis();
 *      if (now - last_toggle >= 100)   ->  toggle the LED, remember when
 *      ...check the next LED...        ->  the loop runs thousands of
 *                                          times per second, never stuck
 *
 *  This is how real firmware does many things "at the same time".
 *
 *  WIRING: 4 LEDs, each wired the same way:
 *
 *      board pin --> LED long leg (+)   LED short leg (-) --> 220-330 ohm --> GND
 *
 *  The button is the BLUE "USER" button already on the Nucleo (PC13).
 *
 *      LED       board pin   blink every
 *      LED 0     A0  (PA0)   100 ms
 *      LED 1     A1  (PA1)   250 ms
 *      LED 2     A2  (PA4)   500 ms
 *      LED 3     D12 (PA6)   1000 ms
 *
 *  TIM1 is already set up for you as a millisecond clock: millis() tells
 *  you how many ms have passed. You only write the blinking logic.
 *
 *    STEP A:  blink LED 0 every 100 ms with millis(), no delay at all.
 *             DONE WHEN: LED 0 blinks fast and steady.
 *    STEP B:  use the arrays to blink all 4 LEDs at their own rates.
 *             DONE WHEN: all 4 blink at once. LED 3 and LED 2 should flip
 *             together every second, because 1000 = 2 x 500.
 *    STEP C:  light PA5 (green) while the button is held.
 *             DONE WHEN: PA5 reacts instantly, even while LED 3 is in the
 *             middle of its long 1 s wait. Try that with delay_ms()!
 *
 *    STRETCH — speed control: every button PRESS halves all 4 periods.
 *             Below 25 ms, go back to normal.
 *             - A PRESS is "pressed now, but NOT pressed last time around
 *               the loop" (edge detection). Keep the last reading in a
 *               variable like  int was_pressed = 0;
 *             - Remove the `const` from PERIOD_MS so you can change it.
 * =============================================================================
 */
#include "esdc.h"

/* GPIOC: the blue button */
#define GPIOC_BASE    0x40020800U
#define GPIOC_MODER   (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_IDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x10U))
#define BUTTON_PIN    13U

static const uint32_t LED_PINS[4]  = {   0,   1,   4,    6 };
static const uint16_t PERIOD_MS[4] = { 100, 250, 500, 1000 };

/* ---------------------------------------------------------------------------
 *  TIM1 as a millisecond clock.  (Done for you.)
 *
 *      16 MHz / 16000 = 1000 ticks per second  ->  CNT goes up once per ms
 *
 *  It's never reset, so CNT works like a wall clock.
 * ------------------------------------------------------------------------ */
static void tim1_init(void)
{
    RCC_APB2ENR |= (1U << 0);       /* TIM1 clock on (it's on the APB2 bus)  */
    TIM1_PSC = 16000U - 1U;         /* 16 MHz / 16000 = 1 kHz: 1 tick per ms */
    TIM1_ARR = 0xFFFFU;             /* count to the 16-bit max, then wrap    */
    TIM1_EGR = TIM1_EGR_UG;         /* make the new PSC take effect now...   */
    TIM1_SR &= ~TIM1_SR_UIF;        /* ...and clear the flag that UG sets    */
    TIM1_CR1 |= TIM1_CR1_CEN;       /* start counting                        */
}

/* ---------------------------------------------------------------------------
 *  millis(): "what time is it?" in ms.  (Done for you.)
 *
 *  CNT is 16 bits, so this clock wraps from 65535 back to 0 about once a
 *  minute. That's fine as long as you always SUBTRACT times:
 *
 *      elapsed = (uint16_t)(now - then);
 *
 *  Example across the wrap:  then = 65500, now = 36  (the clock wrapped)
 *      36 - 65500 = -65464 ... cast to uint16_t ... = 72 ms.  Correct!
 *
 *  The (uint16_t) cast matters: C turns uint16_t math into int math, and
 *  the cast squeezes the answer back into 16 bits.
 * ------------------------------------------------------------------------ */
static uint16_t millis(void)
{
    return (uint16_t)TIM1_CNT;
}

static int button_is_pressed(void)
{
    return ((GPIOC_IDR & (1U << BUTTON_PIN)) == 0U);   /* active low */
}

static void setup(void)
{
    RCC_AHB1ENR |= (1U << 0);       /* GPIOA clock (LEDs)   */
    RCC_AHB1ENR |= (1U << 2);       /* GPIOC clock (button) */

    for (int i = 0; i < 4; i++) {   /* 4 LEDs = outputs     */
        GPIOA_MODER &= ~(3U << (LED_PINS[i] * 2U));
        GPIOA_MODER |=  (1U << (LED_PINS[i] * 2U));
    }
    GPIOA_MODER &= ~(3U << (5U * 2U));          /* PA5 = output */
    GPIOA_MODER |=  (1U << (5U * 2U));
    GPIOC_MODER &= ~(3U << (BUTTON_PIN * 2U));  /* PC13 = input */

    tim1_init();
}

int main(void)
{
    setup();

    /* When each LED was last toggled. Start them all at "now". */
    uint16_t last_toggle[4];
    for (int i = 0; i < 4; i++) {
        last_toggle[i] = millis();
    }

    while (1) {
        uint16_t now = millis();

        /* ---- STEP A --------------------------------------------------------
         * TODO: if at least PERIOD_MS[0] ms have passed since last_toggle[0]:
         *           toggle LED 0 (pin LED_PINS[0])
         *           move last_toggle[0] forward by PERIOD_MS[0]
         *
         * Hint 1: "ms passed since last_toggle[0]" is
         *             (uint16_t)(now - last_toggle[0])
         * Hint 2: why  last_toggle[0] += PERIOD_MS[0];  and not
         *              last_toggle[0] = now;  ?
         *         If the check runs 1 ms late, "= now" makes every blink
         *         after it 1 ms late too, and the errors add up (drift).
         *         "+= period" keeps the blinks on a perfect 100 ms grid.
         */


        /* ---- STEP B --------------------------------------------------------
         * TODO: turn Step A into a for loop over all 4 LEDs (i = 0..3).
         *       Replace every [0] with [i]. Then delete Step A.
         */


        /* ---- STEP C --------------------------------------------------------
         * TODO: if button_is_pressed(), turn PA5 on. Otherwise turn it off.
         */

        /* Silence "unused" warnings until you use these. Delete when done. */
        (void)now; (void)last_toggle; (void)PERIOD_MS; (void)button_is_pressed;
    }
}
