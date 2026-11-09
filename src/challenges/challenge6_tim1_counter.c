/*
 * =============================================================================
 *  CHALLENGE 6 — An ACCURATE delay with TIM1           (Meeting 4: Timers)
 *                Way #2: do everything in the timer
 * =============================================================================
 *
 *  Slides: slides/meeting4_clocks_and_timers.pdf  ("Hands on Time")
 *
 *  Challenge 5 made the timer overflow every 1 ms and counted overflows.
 *  This time we slow the timer down so CNT itself counts milliseconds.
 *  Then "wait 300 ms" is just "reset CNT, wait until CNT reaches 300".
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
 *  REGISTERS: same as Challenge 5 (include/esdc_registers.h).
 *
 *    STEP A:  fill in tim1_init()      (same steps as Challenge 5, new numbers)
 *    STEP B:  fill in tim1_delay_ms()  (only 2 lines!)
 *    DONE WHEN: PA5 does a "heartbeat": two quick blinks, then a pause.
 *               One heartbeat takes exactly 1 second, so 10 heartbeats
 *               should take 10 seconds on your stopwatch.
 *
 *    STEP C:  call tim1_delay_ms(70000) (70 s). What happens, and why?
 *             Fix tim1_delay_ms() so long delays work too.
 *             Hint: 70000 = 60000 + 10000.
 *
 *    THINK:   which way is better, Challenge 5's or this one?
 *               - Which one could also do microsecond delays?
 *               - Which one can wait longer without extra code?
 *               - (Harder) TIM1_CNT = 0 resets the counter, but NOT the
 *                 prescaler's own hidden count from 0 to 15999. So this
 *                 delay can be up to 1 ms short. Does Challenge 5 have the
 *                 same problem? How could writing UG fix it?
 * =============================================================================
 */
#include "esdc.h"

/* ---------------------------------------------------------------------------
 *  STEP A — make CNT count milliseconds
 * ------------------------------------------------------------------------ */
static void tim1_init(void)
{
    /* TODO 1: turn on TIM1's clock (bit 0 of RCC_APB2ENR).                  */


    /* TODO 2: PSC so that CNT ticks once per MILLISECOND this time.
     *         16,000,000 ticks/s / 1,000 ms/s = ? ticks per ms. Then - 1.   */


    /* TODO 3: ARR = 0xFFFF, the largest value a 16-bit register can hold.
     *         (Its reset value is already 0xFFFF. Set it anyway: never
     *          rely on a default you didn't check.)                         */


    /* TODO 4: the GOTCHA from Challenge 5: write UG to load PSC now, then
     *         clear UIF.                                                    */


    /* TODO 5: start counting (CEN bit in TIM1_CR1).                        */

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
