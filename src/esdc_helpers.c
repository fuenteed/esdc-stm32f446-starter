/*
 * =============================================================================
 *  esdc_helpers.c  —  Small helper functions shared by every challenge.
 *  You don't need to edit this file. It's here so you can see that nothing
 *  is hidden: it's just more register writes.
 * =============================================================================
 */
#include "esdc.h"

/* A "busy-wait" delay: the CPU just counts in a loop, doing nothing useful,
 * until enough time has passed. No timer, no interrupts — only burned cycles.
 *
 * Each pass of the inner loop below compiles to 3 instructions
 * (nop, subs, bne) which take about 4 CPU cycles on the Cortex-M4.
 * At 16 MHz that's 16,000 cycles per millisecond / 4 = 4,000 loops.
 * It's approximate: if your LED blinks a bit fast or slow, tweak this.  */
#define LOOPS_PER_MS   (ESDC_CPU_HZ / 1000U / 4U)

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++) {
        for (uint32_t j = 0; j < LOOPS_PER_MS; j++) {
            /* An empty loop gets deleted by the compiler because it "does
             * nothing". A volatile asm "nop" forces it to keep the loop.  */
            __asm volatile ("nop");
        }
    }
}
