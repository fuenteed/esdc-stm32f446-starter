/*
 * =============================================================================
 *  CHALLENGE 6 — An ACCURATE delay with TIM1           (Meeting 4: Timers)
 *                Way #2: do everything in the timer
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
 *  A timer is a counter (CNT) fed by a clock. The PRESCALER (PSC) slows
 *  the 16 MHz clock down first: CNT goes up once every PSC + 1 clock ticks.
 *  We'll slow it down so CNT itself counts milliseconds. Then "wait 300 ms"
 *  is just "reset CNT, wait until CNT reaches 300".
 *
 *  THE PLAN
 *      PSC = 16000 - 1   16 MHz / 16000 = 1000 Hz  ->  CNT goes up once per ms
 *      ARR = 0xFFFF      the biggest 16-bit value  ->  count as high as we can
 *                        (65,535 ms, about 65 s) before overflowing
 *
 *      CNT:  0    1    2    3   ...  299  300          <- 1 ms per step
 *            ^ CNT = 0                     ^ CNT == 300: 300 ms have passed
 *
 *  WIRING: none. Onboard green LED (PA5).
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
 *    STEP A:  fill in tim1_init()      (5 small TODOs)
 *    STEP B:  fill in tim1_delay_ms()  (only 2 lines!)
 *    DONE WHEN: PA5 does a "heartbeat": two quick blinks, then a pause.
 *               One heartbeat takes exactly 1 second, so 10 heartbeats
 *               should take 10 seconds on your stopwatch.
 *
 *    STEP C:  call tim1_delay_ms(70000) (70 s). What happens, and why?
 *             Fix tim1_delay_ms() so long delays work too.
 *             Hint: 70000 = 60000 + 10000.
 *
 *    THINK:   another way to build this delay: tick every microsecond
 *             (PSC = 16 - 1), overflow every 1000 ticks (ARR = 1000 - 1),
 *             and count 1 ms overflows with UIF. Which way is better?
 *               - Which one could also do microsecond delays?
 *               - Which one can wait longer without extra code?
 *               - (Harder) TIM1_CNT = 0 resets the counter, but NOT the
 *                 prescaler's own hidden count from 0 to 15999. So this
 *                 delay can be up to 1 ms short. How could writing UG
 *                 fix it?
 * =============================================================================
 */
#include "esdc.h"

/* ---------------------------------------------------------------------------
 *  STEP A — make CNT count milliseconds
 * ------------------------------------------------------------------------ */
static void tim1_init(void)
{
    /* TODO 1: DON'T FORGET: turn on TIM1's clock. Every peripheral starts
     *         switched off. TIM1 = bit 0 of RCC_APB2ENR.
     *         (A timer with no clock never counts, and your delay waits
     *          forever.)                                                    */


    /* TODO 2: PSC so that CNT ticks once per MILLISECOND this time.
     *         16,000,000 ticks/s / 1,000 ms/s = ? ticks per ms. Then - 1.
     *         Why "- 1"? The timer divides by PSC + 1. PSC = 0 means
     *         "divide by 1".                                                */


    /* TODO 3: ARR = 0xFFFF, the largest value a 16-bit register can hold.
     *         (Its reset value is already 0xFFFF. Set it anyway: never
     *          rely on a default you didn't check.)                         */


    /* TODO 4: GOTCHA! A new PSC value is only used after the next "update
     *         event". Force one now by writing the UG bit, then clear the
     *         UIF flag that the update event just set:
     *             TIM1_EGR = TIM1_EGR_UG;
     *             TIM1_SR &= ~TIM1_SR_UIF;
     *         (Skip this and the new PSC may NEVER load: this delay resets
     *          CNT before it ever overflows. Every delay would then run
     *          16,000 times too fast.)                                      */


    /* TODO 5: start counting: set the CEN bit in TIM1_CR1.
     *         Use |= so you don't change the other bits.                    */

}

/* ---------------------------------------------------------------------------
 *  STEP B — wait until the counter reaches `ms`
 * ------------------------------------------------------------------------ */
static void tim1_delay_ms(uint32_t ms)
{
    /* TODO 1: set TIM1_CNT back to 0.
     * TODO 2: wait while TIM1_CNT is less than ms.
     *
     * Hint: an empty while loop that keeps re-reading the register:
     *           while (TIM1_CNT < ms) { }
     *       It works because TIM1_CNT is "volatile": the compiler must
     *       re-read the real hardware every time around the loop.
     */
    (void)ms;   /* delete this line once you use ms */
}

int main(void)
{
    RCC_AHB1ENR |= (1U << 0);           /* GPIOA clock on    */
    GPIOA_MODER &= ~(3U << (5U * 2U));  /* PA5 = output (01) */
    GPIOA_MODER |=  (1U << (5U * 2U));

    tim1_init();

    /* Heartbeat: 100 + 100 + 100 + 700 = 1000 ms per beat. (Done for you.) */
    while (1) {
        GPIOA_ODR |=  (1U << 5);  tim1_delay_ms(100);   /* ba-   */
        GPIOA_ODR &= ~(1U << 5);  tim1_delay_ms(100);
        GPIOA_ODR |=  (1U << 5);  tim1_delay_ms(100);   /* -dum  */
        GPIOA_ODR &= ~(1U << 5);  tim1_delay_ms(700);   /* rest  */
    }
}
