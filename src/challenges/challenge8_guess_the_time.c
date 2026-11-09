/*
 * =============================================================================
 *  CHALLENGE 8 — SOLUTION   (Guess the Time)
 * =============================================================================
 *  Steps A-B plus the stretch. Set RANDOM_TARGET to 0 for the plain
 *  5-second game.
 *
 *  With RANDOM_TARGET = 1, each round:
 *    1. Press: the target (2-9 s) appears in binary on the 4 LEDs
 *       (LED 0 = 1s bit ... LED 3 = 8s bit). Read it.
 *    2. Press: LEDs off, PA5 on. Start counting.
 *    3. Press at the target. Score shows as before.
 * =============================================================================
 */
#include "esdc.h"

#define GPIOC_BASE    0x40020800U
#define GPIOC_MODER   (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_IDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x10U))
#define BUTTON_PIN    13U

#define TARGET_MS      5000U
#define RANDOM_TARGET  1

static const uint32_t LED_PINS[4] = { 0, 1, 4, 6 };
#define ALL_LEDS  ((1U << 0) | (1U << 1) | (1U << 4) | (1U << 6))

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

static void wait_ms(uint16_t ms)
{
    uint16_t start = millis();
    while ((uint16_t)(millis() - start) < ms) { }
}

static int button_is_pressed(void)
{
    return ((GPIOC_IDR & (1U << BUTTON_PIN)) == 0U);
}

static void wait_for_press(void)
{
    while (button_is_pressed()) { }
    wait_ms(20);
    while (!button_is_pressed()) { }
}

static void show_score(int count)
{
    GPIOA_ODR &= ~ALL_LEDS;
    for (int i = 0; i < count; i++) {
        GPIOA_ODR |= (1U << LED_PINS[i]);
    }
}

/* Stretch: LED i shows bit i of value (0-15). */
static void show_binary(uint32_t value)
{
    GPIOA_ODR &= ~ALL_LEDS;
    for (int i = 0; i < 4; i++) {
        if (value & (1U << i)) {
            GPIOA_ODR |= (1U << LED_PINS[i]);
        }
    }
}

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

    while (1) {
        wait_for_press();

        uint32_t target_ms = TARGET_MS;
#if RANDOM_TARGET
        /* The ms value at the moment a human presses is close to random. */
        uint32_t target_s = (millis() % 8U) + 2U;     /* 2..9 */
        target_ms = target_s * 1000U;
        show_binary(target_s);
        wait_for_press();
#else
        (void)show_binary;
#endif

        show_score(0);
        GPIOA_ODR |= (1U << 5);

        /* Step A */
        uint16_t start = millis();
        wait_for_press();
        uint32_t elapsed = (uint16_t)(millis() - start);

        GPIOA_ODR &= ~(1U << 5);

        /* Step B */
        uint32_t error;
        if (elapsed > target_ms) { error = elapsed - target_ms; }
        else                     { error = target_ms - elapsed; }

        int score;
        if      (error < 100U)  { score = 4; }
        else if (error < 250U)  { score = 3; }
        else if (error < 500U)  { score = 2; }
        else if (error < 1000U) { score = 1; }
        else                    { score = 0; }

        if (score > 0) {
            show_score(score);
        } else {
            flash_all();
        }
    }
}
