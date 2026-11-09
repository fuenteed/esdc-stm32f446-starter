/*
 * =============================================================================
 *  CHALLENGE 8 (STRETCH GAME) — Guess the Time         (Meeting 4: Timers)
 * =============================================================================
 *
 *  Your brain vs. TIM1. Press the button to start, count 5 seconds in your
 *  head (no looking at a clock!), press again. The timer knows exactly how
 *  close you got.
 *
 *    1. Press the blue button.     PA5 (green) turns on: start counting.
 *    2. Press again at "5".        PA5 turns off.
 *    3. The 4 LEDs show your score:
 *           4 LEDs = within 100 ms      2 LEDs = within 500 ms
 *           3 LEDs = within 250 ms      1 LED  = within 1 s
 *           all 4 flashing = more than 1 s off. Try again!
 *    4. Press to play again.
 *
 *  WIRING: 4 LEDs on A0 (PA0), A1 (PA1), A2 (PA4) and D12 (PA6), each:
 *
 *      board pin --> LED long leg (+)   LED short leg (-) --> 220-330 ohm --> GND
 *
 *  The button is the BLUE "USER" button already on the Nucleo (PC13).
 *
 *  TIM1, the button and the LEDs are all set up for you. millis() tells you
 *  how many ms have passed. You only write the timing and scoring.
 *
 *    STEP A:  measure how long the player waited (between the two presses).
 *    STEP B:  turn that into an error ("how far off") and a score (0-4).
 *    DONE WHEN: you can play it. Wait one round with your phone's
 *               stopwatch running and check that the score is believable.
 *               Then beat your partner.
 *
 *    STRETCH — random target: pick a target between 2 and 9 seconds each
 *             round and show it in BINARY on the 4 LEDs before the round
 *             starts (5 = 0101 -> LED 0 and LED 2 on). Where do you get a
 *             random number with no random() function? Hint: the exact ms
 *             value of TIM1_CNT when a human presses a button is pretty
 *             random. Try (millis() % 8) + 2.
 * =============================================================================
 */
#include "esdc.h"

/* GPIOC: the blue button */
#define GPIOC_BASE    0x40020800U
#define GPIOC_MODER   (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_IDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x10U))
#define BUTTON_PIN    13U

#define TARGET_MS     5000U

static const uint32_t LED_PINS[4] = { 0, 1, 4, 6 };
#define ALL_LEDS  ((1U << 0) | (1U << 1) | (1U << 4) | (1U << 6))

/* ===========================================================================
 *  Helpers. All done for you, but read them: they're what makes it work.
 * ======================================================================== */

/* TIM1 as a millisecond clock:
 *     16 MHz / 16000 = 1000 ticks per second  ->  CNT goes up once per ms
 * It's never reset, so CNT works like a wall clock.                        */
static void tim1_init(void)
{
    RCC_APB2ENR |= (1U << 0);       /* TIM1 clock on (it's on the APB2 bus)  */
    TIM1_PSC = 16000U - 1U;         /* 16 MHz / 16000 = 1 kHz: 1 tick per ms */
    TIM1_ARR = 0xFFFFU;             /* count to the 16-bit max, then wrap    */
    TIM1_EGR = TIM1_EGR_UG;         /* make the new PSC take effect now...   */
    TIM1_SR &= ~TIM1_SR_UIF;        /* ...and clear the flag that UG sets    */
    TIM1_CR1 |= TIM1_CR1_CEN;       /* start counting                        */
}

/* "What time is it?" in ms. CNT is 16 bits, so this clock wraps from 65535
 * back to 0 about once a minute. That's fine as long as you SUBTRACT:
 *
 *     elapsed = (uint16_t)(now - then);
 *
 * Example across the wrap:  then = 65500, now = 36  (the clock wrapped)
 *     36 - 65500 = -65464 ... cast to uint16_t ... = 72 ms.  Correct!
 * The cast matters: C does uint16_t math as int, and the cast squeezes the
 * answer back into 16 bits.                                               */
static uint16_t millis(void)
{
    return (uint16_t)TIM1_CNT;
}

/* Wait `ms` milliseconds WITHOUT resetting CNT (resetting it would break
 * millis()). It just watches the clock go by.                             */
static void wait_ms(uint16_t ms)
{
    uint16_t start = millis();
    while ((uint16_t)(millis() - start) < ms) { }
}

static int button_is_pressed(void)
{
    return ((GPIOC_IDR & (1U << BUTTON_PIN)) == 0U);   /* active low */
}

/* Wait for a NEW press: first wait for the last press to be let go, then
 * for the button to go down again. The 20 ms wait skips "contact bounce":
 * a real button flickers on/off for a few ms when it moves.               */
static void wait_for_press(void)
{
    while (button_is_pressed()) { }
    wait_ms(20);
    while (!button_is_pressed()) { }
}

/* Light the first `count` LEDs (0 to 4) and turn the rest off. */
static void show_score(int count)
{
    GPIOA_ODR &= ~ALL_LEDS;
    for (int i = 0; i < count; i++) {
        GPIOA_ODR |= (1U << LED_PINS[i]);
    }
}

/* Flash all 4 LEDs a few times: "way off, try again". */
static void flash_all(void)
{
    for (int i = 0; i < 6; i++) {
        GPIOA_ODR ^= ALL_LEDS;
        wait_ms(150);
    }
    GPIOA_ODR &= ~ALL_LEDS;
}

static void setup(void)
{
    RCC_AHB1ENR |= (1U << 0);       /* GPIOA clock (LEDs)   */
    RCC_AHB1ENR |= (1U << 2);       /* GPIOC clock (button) */

    for (int i = 0; i < 4; i++) {
        GPIOA_MODER &= ~(3U << (LED_PINS[i] * 2U));
        GPIOA_MODER |=  (1U << (LED_PINS[i] * 2U));
    }
    GPIOA_MODER &= ~(3U << (5U * 2U));          /* PA5 = output */
    GPIOA_MODER |=  (1U << (5U * 2U));
    GPIOC_MODER &= ~(3U << (BUTTON_PIN * 2U));  /* PC13 = input */

    tim1_init();
}

/* ===========================================================================
 *  The game
 * ======================================================================== */
int main(void)
{
    setup();

    while (1) {
        /* ---- Round start ---------------------------------------------- */
        wait_for_press();
        show_score(0);                  /* clear last round's score */
        GPIOA_ODR |= (1U << 5);         /* green on: start counting! */

        /* ---- STEP A --------------------------------------------------------
         * TODO: measure how long until the next press.
         *   1. save the time NOW:          uint16_t start = millis();
         *   2. wait for the next press:    wait_for_press();
         *   3. elapsed = time now minus start (with the uint16_t cast!)
         */
        uint32_t elapsed = 0;    /* TODO: replace 0 */

        GPIOA_ODR &= ~(1U << 5);        /* green off: time's up */

        /* ---- STEP B --------------------------------------------------------
         * TODO: how far off was the player? Then score it.
         *
         * error = the distance between elapsed and TARGET_MS, always
         *         positive. 4800 ms and 5200 ms are both 200 ms off.
         *
         * Hint 1: unsigned numbers can't go negative! 4800 - 5000 is NOT
         *         -200 in a uint32_t: it wraps to about 4.29 billion.
         *         So check which one is bigger first:
         *             if (elapsed > TARGET_MS) { error = elapsed - TARGET_MS; }
         *             else                     { error = TARGET_MS - elapsed; }
         *
         * Hint 2: then an if / else if chain sets score:
         *             error < 100  -> 4
         *             error < 250  -> 3     ... and so on (see the top).
         */
        uint32_t error = 0;      /* TODO */
        int      score = 0;      /* TODO */
        (void)elapsed;
        (void)error;

        if (score > 0) {
            show_score(score);
        } else {
            flash_all();
        }
    }
}
