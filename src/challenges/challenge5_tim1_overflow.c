/*
 * =============================================================================
 *  CHALLENGE 5 — SOLUTION   (Way #1: overflow every 1 ms, count overflows)
 * =============================================================================
 *  main() toggles PA5 every 500 ms with tim1_delay_ms().
 *  For the stretch, set USE_US_STRETCH to 1: PA5 toggles every 250 us
 *  (2 kHz square wave), which looks like a dim, steady LED. The eye
 *  averages it: on half the time = about half brightness. That's PWM.
 * =============================================================================
 */
#include "esdc.h"

#define BLINK_MS         500U
#define USE_US_STRETCH   0

static void tim1_init(void)
{
    RCC_APB2ENR |= (1U << 0);           /* 1. TIM1 clock on                  */
    TIM1_PSC = 16U - 1U;                /* 2. 16 MHz / 16 = 1 MHz: 1 tick/us */
    TIM1_ARR = 1000U - 1U;              /* 3. 0..999 = 1000 us = 1 ms        */
    TIM1_EGR = TIM1_EGR_UG;             /* 4. load PSC now...                */
    TIM1_SR &= ~TIM1_SR_UIF;            /*    ...and clear the UIF it set    */
    TIM1_CR1 |= TIM1_CR1_CEN;           /* 5. go                             */
}

static void tim1_delay_ms(uint32_t ms)
{
    TIM1_CNT = 0U;                      /* fresh millisecond */
    TIM1_SR &= ~TIM1_SR_UIF;

    for (uint32_t i = 0; i < ms; i++) {
        while ((TIM1_SR & TIM1_SR_UIF) == 0U) {
            /* wait for the next 1 ms overflow */
        }
        TIM1_SR &= ~TIM1_SR_UIF;
    }
}

/* STRETCH: 1..999 us. CNT is already counting microseconds and never
 * passes 999, so just reset it and wait. Accurate down to a few us: the
 * reset + loop overhead is a handful of 16 MHz cycles (~0.5 us).         */
static void tim1_delay_us(uint32_t us)
{
    TIM1_CNT = 0U;
    while (TIM1_CNT < us) {
    }
}

int main(void)
{
    RCC_AHB1ENR |= (1U << 0);
    GPIOA_MODER &= ~(3U << (5U * 2U));
    GPIOA_MODER |=  (1U << (5U * 2U));

    tim1_init();

    while (1) {
        GPIOA_ODR ^= (1U << 5);
#if USE_US_STRETCH
        tim1_delay_us(250);
        (void)tim1_delay_ms;
#else
        tim1_delay_ms(BLINK_MS);
        (void)tim1_delay_us;
#endif
    }
}
