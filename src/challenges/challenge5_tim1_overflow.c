/*
 * =============================================================================
 *  CHALLENGE 5 — An ACCURATE delay with TIM1           (Meeting 4: Timers)
 *                Way #1: overflow every 1 ms, count the overflows
 * =============================================================================
 *
 *  Slides: slides/meeting4_clocks_and_timers.pdf  ("Hands on Time")
 *
 *  WHY? Our old delay_ms() (src/esdc_helpers.c) is a busy-wait loop like
 *
 *      for (volatile int i = 0; i < "some big number"; i++) { }
 *
 *  How long is that? Nobody really knows. It depends on how the compiler
 *  turned the loop into instructions. A hardware TIMER counts real clock
 *  ticks, so the time is exact.
 *
 *  THE PLAN
 *      PSC = 16 - 1     16 MHz / 16 = 1 MHz    ->  CNT goes up once per us
 *      ARR = 1000 - 1   counts 0..999 = 1000   ->  overflows once per ms
 *
 *      CNT:  0 1 2 ... 998 999  0 1 2 ... 998 999  0 1 ...
 *                               ^                  ^
 *                            UIF = 1            UIF = 1
 *                            (1 ms)             (2 ms)
 *
 *  To wait N ms: wait for UIF, clear it, and do that N times.
 *
 *  WIRING: none. We blink the onboard green LED (PA5).
 *
 *  REGISTERS (already defined in include/esdc_registers.h; Ctrl/Cmd+click
 *  any name to jump to it)
 *      RCC_APB2ENR  bit 0      TIM1's clock switch (TIM1 is on the APB2 bus)
 *      TIM1_PSC                prescaler:  tick rate = 16 MHz / (PSC + 1)
 *      TIM1_ARR                auto-reload: count up to this, then overflow
 *      TIM1_CNT                the current count
 *      TIM1_EGR     bit 0 UG   "apply my settings right now"
 *      TIM1_SR      bit 0 UIF  1 = the counter overflowed. Clear by writing 0.
 *      TIM1_CR1     bit 0 CEN  1 = start counting
 *
 *    STEP A:  fill in tim1_init()          (5 small TODOs)
 *    STEP B:  fill in tim1_delay_ms()
 *    DONE WHEN: PA5 toggles every 500 ms. Start your phone's stopwatch on a
 *               toggle: 20 toggles should take exactly 10 seconds.
 *
 *    (Until Step B is done, PA5 looks dimly ON. It's toggling millions of
 *     times a second because the delay doesn't wait yet.)
 *
 *    STEP C:  change BLINK_MS to 100, then 2000. Still exact?
 *
 *    STRETCH — tim1_delay_us(): look at the plan again. CNT is ALREADY
 *             counting microseconds! Write a delay for 1 to 999 us, and use
 *             it to toggle PA5 every 250 us. It looks dim instead of
 *             blinking. Why? (That's a preview of next meeting: PWM.)
 * =============================================================================
 */
#include "esdc.h"

#define BLINK_MS   500U     /* Step C: try other values */

/* ---------------------------------------------------------------------------
 *  STEP A — set TIM1 up to overflow once every millisecond
 * ------------------------------------------------------------------------ */
static void tim1_init(void)
{
    /* TODO 1: DON'T FORGET: turn on TIM1's clock. Every peripheral starts
     *         switched off. TIM1 = bit 0 of RCC_APB2ENR.
     *         (A timer with no clock never counts, and your delay waits
     *          forever.)                                                    */


    /* TODO 2: set the prescaler so CNT ticks once per microsecond.
     *         TIM1_PSC = ...;          (see THE PLAN at the top)
     *         Why "- 1"? The timer divides by PSC + 1. PSC = 0 means
     *         "divide by 1".                                                */


    /* TODO 3: set the auto-reload so the counter overflows every 1000 ticks.
     *         TIM1_ARR = ...;
     *         Careful: it counts 0, 1, ... ARR. That's ARR + 1 ticks.       */


    /* TODO 4: GOTCHA! A new PSC value is only used after the next "update
     *         event". Force one now by writing the UG bit, then clear the
     *         UIF flag that the update event just set:
     *             TIM1_EGR = TIM1_EGR_UG;
     *             TIM1_SR &= ~TIM1_SR_UIF;
     *         (Skip this and your first lap runs at 16 MHz instead of 1 MHz.) */


    /* TODO 5: start counting: set the CEN bit in TIM1_CR1.
     *         Use |= so you don't change the other bits.                    */

}

/* ---------------------------------------------------------------------------
 *  STEP B — wait `ms` milliseconds by counting overflows
 * ------------------------------------------------------------------------ */
static void tim1_delay_ms(uint32_t ms)
{
    /* TODO 1: start a fresh millisecond: set TIM1_CNT to 0 and clear UIF.
     *         The timer has been running in the background, so without
     *         this your first "millisecond" could be anywhere from 0 to 1 ms.
     *
     * TODO 2: do this `ms` times:
     *             wait while UIF is still 0
     *             clear UIF
     *
     * Hint 1: a loop that runs ms times:
     *             for (uint32_t i = 0; i < ms; i++) { ... }
     *
     * Hint 2: "wait while UIF is 0" is an empty while loop that keeps
     *         checking the bit:
     *             while ((TIM1_SR & TIM1_SR_UIF) == 0U) { }
     *
     * Hint 3: blinks way too fast after the first one? You forgot to clear
     *         UIF. It's still 1 the next time around, so the while loop
     *         exits instantly.
     */
    (void)ms;   /* delete this line once you use ms */
}

/* ---------------------------------------------------------------------------
 *  STRETCH — wait `us` microseconds (1 to 999)
 * ------------------------------------------------------------------------ */
static void tim1_delay_us(uint32_t us)
{
    /* TODO (stretch): CNT goes up once per microsecond and never passes
     * 999. So: reset CNT to 0, then wait while CNT is less than us.
     *
     * Think: at 16 MHz, 1 us is only 16 clock cycles. What's the shortest
     * delay that can still be accurate?
     */
    (void)us;
}

int main(void)
{
    RCC_AHB1ENR |= (1U << 0);           /* GPIOA clock on      */
    GPIOA_MODER &= ~(3U << (5U * 2U));  /* PA5 = output (01)   */
    GPIOA_MODER |=  (1U << (5U * 2U));

    tim1_init();

    (void)tim1_delay_us;   /* silences "unused" until you use it (stretch) */

    while (1) {
        GPIOA_ODR ^= (1U << 5);         /* toggle the green LED */
        tim1_delay_ms(BLINK_MS);        /* no delay_ms() anywhere! */
    }
}
