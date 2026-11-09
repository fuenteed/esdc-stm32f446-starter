/*
 * =============================================================================
 *  CHALLENGE 6 — SOLUTION   (Way #2: CNT counts milliseconds)
 * =============================================================================
 *  main() runs the 1-second heartbeat on PA5.
 *
 *  THINK answers:
 *    - Microseconds: Way #1 (Challenge 5). Here one tick is a whole ms.
 *    - Long delays: Way #1 counts overflows in a uint32_t loop, so it goes
 *      to ~49 days with no extra code. Way #2 tops out at 65,535 ms unless
 *      you split the delay into chunks (Step C below).
 *    - The prescaler's hidden 0..15999 count isn't reset by CNT = 0, so the
 *      first tick can arrive anywhere from 1/16000 ms to 1 ms later:
 *      the delay is between ms-1 and ms. Challenge 5 has the same issue,
 *      but its prescaler only divides by 16, so the error is under 1 us.
 *      Writing TIM1_EGR = TIM1_EGR_UG instead of CNT = 0 resets BOTH the
 *      counter and the prescaler, so the delay becomes exact.
 * =============================================================================
 */
#include "esdc.h"

#define MAX_CHUNK_MS   60000U    /* comfortably under the 65,535 ms limit */

static void tim1_init(void)
{
    RCC_APB2ENR |= (1U << 0);           /* 1. TIM1 clock on                   */
    TIM1_PSC = 16000U - 1U;             /* 2. 16 MHz / 16000 = 1 kHz: 1 tick/ms */
    TIM1_ARR = 0xFFFFU;                 /* 3. count as high as 16 bits go     */
    TIM1_EGR = TIM1_EGR_UG;             /* 4. load PSC now...                 */
    TIM1_SR &= ~TIM1_SR_UIF;            /*    ...clear the UIF it set         */
    TIM1_CR1 |= TIM1_CR1_CEN;           /* 5. go                              */
}

/* One chunk: up to 65,535 ms. UG resets CNT *and* the prescaler, so the
 * wait is exact (see THINK above). CNT = 0U also works, up to 1 ms short. */
static void tim1_wait_chunk(uint32_t ms)
{
    TIM1_EGR = TIM1_EGR_UG;
    while (TIM1_CNT < ms) {
    }
}

/* Step C: split long delays into chunks the 16-bit counter can reach. */
static void tim1_delay_ms(uint32_t ms)
{
    while (ms > MAX_CHUNK_MS) {
        tim1_wait_chunk(MAX_CHUNK_MS);
        ms -= MAX_CHUNK_MS;
    }
    tim1_wait_chunk(ms);
}

int main(void)
{
    RCC_AHB1ENR |= (1U << 0);
    GPIOA_MODER &= ~(3U << (5U * 2U));
    GPIOA_MODER |=  (1U << (5U * 2U));

    tim1_init();

    while (1) {
        GPIOA_ODR |=  (1U << 5);  tim1_delay_ms(100);
        GPIOA_ODR &= ~(1U << 5);  tim1_delay_ms(100);
        GPIOA_ODR |=  (1U << 5);  tim1_delay_ms(100);
        GPIOA_ODR &= ~(1U << 5);  tim1_delay_ms(700);
    }
}
