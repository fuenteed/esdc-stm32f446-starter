/*
 * =============================================================================
 *  CHALLENGE 7 — SOLUTION   (four blink rates at once, no delays)
 * =============================================================================
 *  Steps A-C plus the stretch: each button PRESS halves every period;
 *  once LED 0 would drop below 25 ms, they all reset to normal.
 *  PA5 is on while the button is held (Step C), with no lag.
 * =============================================================================
 */
#include "esdc.h"

#define GPIOC_BASE    0x40020800U
#define GPIOC_MODER   (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_IDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x10U))
#define BUTTON_PIN    13U

static const uint32_t LED_PINS[4]       = {   0,   1,   4,    6 };
static const uint16_t NORMAL_PERIOD[4]  = { 100, 250, 500, 1000 };
static uint16_t       PERIOD_MS[4]      = { 100, 250, 500, 1000 };   /* stretch: not const */

static void tim1_init(void)
{
    RCC_APB2ENR |= (1U << 0);
    TIM1_PSC = 16000U - 1U;             /* CNT = ms */
    TIM1_ARR = 0xFFFFU;
    TIM1_EGR = TIM1_EGR_UG;
    TIM1_SR &= ~TIM1_SR_UIF;
    TIM1_CR1 |= TIM1_CR1_CEN;
}

static uint16_t millis(void)
{
    return (uint16_t)TIM1_CNT;
}

static int button_is_pressed(void)
{
    return ((GPIOC_IDR & (1U << BUTTON_PIN)) == 0U);
}

static void setup(void)
{
    RCC_AHB1ENR |= (1U << 0);
    RCC_AHB1ENR |= (1U << 2);

    for (int i = 0; i < 4; i++) {
        GPIOA_MODER &= ~(3U << (LED_PINS[i] * 2U));
        GPIOA_MODER |=  (1U << (LED_PINS[i] * 2U));
    }
    GPIOA_MODER &= ~(3U << (5U * 2U));
    GPIOA_MODER |=  (1U << (5U * 2U));
    GPIOC_MODER &= ~(3U << (BUTTON_PIN * 2U));

    tim1_init();
}

int main(void)
{
    setup();

    uint16_t last_toggle[4];
    for (int i = 0; i < 4; i++) {
        last_toggle[i] = millis();
    }

    int      was_pressed   = 0;
    uint16_t last_press_ms = millis();

    while (1) {
        uint16_t now = millis();

        /* Steps A + B: each LED on its own schedule. */
        for (int i = 0; i < 4; i++) {
            if ((uint16_t)(now - last_toggle[i]) >= PERIOD_MS[i]) {
                GPIOA_ODR ^= (1U << LED_PINS[i]);
                last_toggle[i] += PERIOD_MS[i];     /* no drift */
            }
        }

        /* Step C: instant response. */
        int pressed = button_is_pressed();
        if (pressed) {
            GPIOA_ODR |=  (1U << 5);
        } else {
            GPIOA_ODR &= ~(1U << 5);
        }

        /* Stretch: new press (edge) -> halve all periods. The 20 ms
         * lockout ignores contact bounce without ever blocking.        */
        if (pressed && !was_pressed && (uint16_t)(now - last_press_ms) > 20U) {
            last_press_ms = now;
            int reset = (PERIOD_MS[0] / 2U) < 25U;
            for (int i = 0; i < 4; i++) {
                PERIOD_MS[i] = reset ? NORMAL_PERIOD[i] : (uint16_t)(PERIOD_MS[i] / 2U);
                last_toggle[i] = now;               /* restart the grid */
            }
        }
        was_pressed = pressed;
    }
}
