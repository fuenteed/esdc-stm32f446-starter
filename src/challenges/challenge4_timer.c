/*
 * =============================================================================
 *  CHALLENGE 4 (STRETCH) — A real hardware timer   (skill: configuring a peripheral)
 * =============================================================================
 *
 *  delay_ms() makes the CPU spin in a loop. A hardware TIMER is a separate
 *  circuit that counts on its own, so the CPU just checks "is it time yet?".
 *  Later, timers can even interrupt the CPU so it never has to check.
 *
 *  We'll use TIM2, a 32-bit timer (RM0390 Chapter 18, "General-purpose
 *  timers TIM2 to TIM5"). How it works:
 *
 *      16 MHz clock --> [ PRESCALER (PSC) ] --> slow ticks --> [ COUNTER (CNT) ]
 *                         divide by PSC+1                     counts 0..ARR,
 *                                                             then wraps to 0
 *                                                             and sets UIF = 1
 *
 *      PSC = 16000 - 1  ->  16,000,000 / 16,000 = 1,000 ticks/s = 1 tick per ms
 *      ARR = 500 - 1    ->  counter wraps every 500 ticks = every 500 ms
 *
 *  WIRING: none. We blink the onboard green LED (PA5).
 *
 *    STEP A:  fill in timer_init()  (6 small steps below)
 *    STEP B:  fill in timer_wait()
 *    DONE WHEN: the green LED toggles every 500 ms. Check with a stopwatch:
 *               20 toggles should take exactly 10 seconds.
 *
 *    STRETCH — Reaction timer: wait a few seconds, turn PA5 on, and measure
 *             how long until the button (PC13) is pressed by reading TIM2_CNT
 *             (with ARR set to its max, 0xFFFFFFFF, CNT is just "ms since
 *             start"). Show the result on the 4 Challenge 1 LEDs:
 *             4 LEDs = under 200 ms, 3 = under 300 ms, 2 = under 400 ms ...
 * =============================================================================
 */
#include "esdc.h"

/* TIM2 registers (RM0390 Section 18.4, register map at the end of 18.4) */
#define TIM2_BASE     0x40000000U
#define TIM2_CR1      (*(volatile uint32_t *)(TIM2_BASE + 0x00U))  /* control: bit 0 CEN = counter enable   */
#define TIM2_SR       (*(volatile uint32_t *)(TIM2_BASE + 0x10U))  /* status:  bit 0 UIF = "counter wrapped" */
#define TIM2_EGR      (*(volatile uint32_t *)(TIM2_BASE + 0x14U))  /* event generation: bit 0 UG            */
#define TIM2_CNT      (*(volatile uint32_t *)(TIM2_BASE + 0x24U))  /* the counter itself                    */
#define TIM2_PSC      (*(volatile uint32_t *)(TIM2_BASE + 0x28U))  /* prescaler                             */
#define TIM2_ARR      (*(volatile uint32_t *)(TIM2_BASE + 0x2CU))  /* auto-reload ("count up to")           */

#define TIM_CR1_CEN   (1U << 0)
#define TIM_SR_UIF    (1U << 0)
#define TIM_EGR_UG    (1U << 0)

/* ---------------------------------------------------------------------------
 *  STEP A — set TIM2 up to "wrap" every period_ms milliseconds
 * ------------------------------------------------------------------------ */
static void timer_init(uint32_t period_ms)
{
    (void)period_ms;   /* delete this line once you use period_ms below */

    /* TODO 1: turn on TIM2's clock. TIM2 is on the APB1 bus, so the enable
     *         bit is in RCC_APB1ENR (not AHB1ENR!). TIM2 = bit 0.           */

    /* TODO 2: set the prescaler so the counter ticks once per millisecond.
     *         TIM2_PSC = ...;   (see the math at the top of the file)      */

    /* TODO 3: set the auto-reload value so it wraps every period_ms ticks.
     *         Careful: it counts 0..ARR, which is ARR+1 ticks.             */

    /* TODO 4: start counting from zero:  TIM2_CNT = 0;                     */

    /* TODO 5: GOTCHA! The prescaler value doesn't actually take effect until
     *         the next "update event". Force one now by setting the UG bit
     *         in TIM2_EGR. That also sets UIF, so clear UIF right after:
     *             TIM2_EGR = TIM_EGR_UG;
     *             TIM2_SR &= ~TIM_SR_UIF;                                    */

    /* TODO 6: start the timer: set the CEN bit in TIM2_CR1.                */
}

/* ---------------------------------------------------------------------------
 *  STEP B — wait until the timer wraps, then clear the flag
 * ------------------------------------------------------------------------ */
static void timer_wait(void)
{
    /* TODO:
     *   1. loop while the UIF bit in TIM2_SR is still 0
     *   2. clear UIF (if you don't, the next wait returns instantly!)
     *
     * Hint: while ((TIM2_SR & TIM_SR_UIF) == 0U) { }
     */
}

int main(void)
{
    RCC_AHB1ENR |= (1U << 0);
    GPIOA_MODER &= ~(3U << (5U * 2U));  /* PA5 = output (01) */
    GPIOA_MODER |=  (1U << (5U * 2U));

    timer_init(500);

    while (1) {
        timer_wait();               /* no delay_ms() anywhere! */
        GPIOA_ODR ^= (1U << 5);
    }
}
