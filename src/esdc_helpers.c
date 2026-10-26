/*
 * =============================================================================
 *  esdc_helpers.c  —  Small helper functions shared by every challenge.
 *  You don't need to edit this file. It's here so you can see that nothing
 *  is hidden: it's just more register writes.
 * =============================================================================
 */
#include "esdc.h"

/* SysTick is a simple 24-bit countdown timer inside the ARM Cortex-M4 core
 * (not an STM32 peripheral, so it isn't in RM0390 — it's in ST's PM0214
 * programming manual). It counts down from LOAD to 0, then reloads.
 * Each time it hits 0 it sets COUNTFLAG (bit 16 of CTRL).               */
#define SYSTICK_CTRL   (*(volatile uint32_t *)0xE000E010U)
#define SYSTICK_LOAD   (*(volatile uint32_t *)0xE000E014U)
#define SYSTICK_VAL    (*(volatile uint32_t *)0xE000E018U)

#define SYSTICK_ENABLE       (1U << 0)
#define SYSTICK_CLK_CPU      (1U << 2)    /* count at the CPU clock (16 MHz) */
#define SYSTICK_COUNTFLAG    (1U << 16)   /* 1 = reached zero since last read */

void delay_ms(uint32_t ms)
{
    /* 16,000,000 ticks per second / 1000 = 16,000 ticks per millisecond.
     * The timer counts LOAD..0, which is LOAD+1 ticks, so subtract 1.    */
    SYSTICK_LOAD = (ESDC_CPU_HZ / 1000U) - 1U;
    SYSTICK_VAL  = 0U;                           /* start from the top    */
    SYSTICK_CTRL = SYSTICK_CLK_CPU | SYSTICK_ENABLE;

    for (uint32_t i = 0; i < ms; i++) {
        /* Wait for one millisecond to pass. Reading CTRL clears the flag. */
        while ((SYSTICK_CTRL & SYSTICK_COUNTFLAG) == 0U) {
            /* spin */
        }
    }

    SYSTICK_CTRL = 0U;                           /* stop the timer        */
}
