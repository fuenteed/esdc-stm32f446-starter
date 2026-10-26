/*
 * =============================================================================
 *  CHALLENGE 4 — SOLUTION
 * =============================================================================
 *  main() blinks PA5 every 500 ms using TIM2 instead of delay_ms().
 *  For the reaction-timer stretch, change main() to call reaction_game().
 *  (Needs the 4 Challenge 1 LEDs on A0, A1, A2, D12.)
 * =============================================================================
 */
#include "esdc.h"

#define TIM2_BASE     0x40000000U
#define TIM2_CR1      (*(volatile uint32_t *)(TIM2_BASE + 0x00U))
#define TIM2_SR       (*(volatile uint32_t *)(TIM2_BASE + 0x10U))
#define TIM2_EGR      (*(volatile uint32_t *)(TIM2_BASE + 0x14U))
#define TIM2_CNT      (*(volatile uint32_t *)(TIM2_BASE + 0x24U))
#define TIM2_PSC      (*(volatile uint32_t *)(TIM2_BASE + 0x28U))
#define TIM2_ARR      (*(volatile uint32_t *)(TIM2_BASE + 0x2CU))

#define TIM_CR1_CEN   (1U << 0)
#define TIM_SR_UIF    (1U << 0)
#define TIM_EGR_UG    (1U << 0)

static void timer_init(uint32_t period_ms)
{
    RCC_APB1ENR |= (1U << 0);                  /* 1. TIM2 clock on               */
    TIM2_PSC = (ESDC_CPU_HZ / 1000U) - 1U;     /* 2. 16 MHz / 16000 = 1 kHz      */
    TIM2_ARR = period_ms - 1U;                 /* 3. counts 0..ARR = period_ms   */
    TIM2_CNT = 0U;                             /* 4. start at zero               */
    TIM2_EGR = TIM_EGR_UG;                     /* 5. load PSC now...             */
    TIM2_SR &= ~TIM_SR_UIF;                    /*    ...and clear the UIF it set */
    TIM2_CR1 |= TIM_CR1_CEN;                   /* 6. go                          */
}

static void timer_wait(void)
{
    while ((TIM2_SR & TIM_SR_UIF) == 0U) {
        /* wait for the counter to wrap */
    }
    TIM2_SR &= ~TIM_SR_UIF;
}

/* ---------------------------------------------------------------------------
 *  STRETCH: reaction timer
 * ------------------------------------------------------------------------ */
#define GPIOC_BASE    0x40020800U
#define GPIOC_IDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x10U))

static const uint32_t LED_PINS[4] = { 0, 1, 4, 6 };
#define ALL_LEDS  ((1U << 0) | (1U << 1) | (1U << 4) | (1U << 6))

static int button_is_pressed(void)
{
    return ((GPIOC_IDR & (1U << 13)) == 0U);
}

static void reaction_game(void)
{
    RCC_AHB1ENR |= (1U << 2);                  /* GPIOC clock for the button */
    for (int i = 0; i < 4; i++) {
        PIN_OUTPUT(GPIOA_MODER, LED_PINS[i]);
    }

    /* Free-running 1 ms counter: ARR at max, so CNT = ms since start. */
    timer_init(0xFFFFFFFFU);   /* ARR = 0xFFFFFFFE, plenty */

    uint32_t round = 0;

    while (1) {
        GPIOA_ODR &= ~((1U << 5) | ALL_LEDS);

        /* Wait 2-4 s. Not truly random, but it changes every round. */
        delay_ms(2000U + (round * 737U) % 2000U);
        round++;

        if (button_is_pressed()) {             /* pressed too early: cheater! */
            for (int i = 0; i < 6; i++) { GPIOA_ODR ^= (1U << 5); delay_ms(100); }
            continue;
        }

        GPIOA_ODR |= (1U << 5);                /* GO! */
        uint32_t start = TIM2_CNT;
        while (!button_is_pressed()) { }
        uint32_t reaction_ms = TIM2_CNT - start;
        GPIOA_ODR &= ~(1U << 5);

        int lit = 0;
        if      (reaction_ms < 200U) { lit = 4; }
        else if (reaction_ms < 300U) { lit = 3; }
        else if (reaction_ms < 400U) { lit = 2; }
        else if (reaction_ms < 600U) { lit = 1; }

        for (int i = 0; i < lit; i++) {
            GPIOA_ODR |= (1U << LED_PINS[i]);
        }
        delay_ms(2000);

        while (button_is_pressed()) { }        /* wait for release */
    }
}

int main(void)
{
    RCC_AHB1ENR |= (1U << 0);
    PIN_OUTPUT(GPIOA_MODER, 5);

    (void)reaction_game;   /* swap this in for the stretch */

    timer_init(500);

    while (1) {
        timer_wait();
        GPIOA_ODR ^= (1U << 5);
    }
}
