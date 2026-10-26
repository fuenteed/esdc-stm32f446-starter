/*
 * =============================================================================
 *  startup_stm32f446.c  —  What runs BEFORE main()
 * =============================================================================
 *  You don't need to edit this for any challenge, but it's worth a read
 *  once you're comfortable. It answers "how does the chip get to main()?"
 *
 *  When the chip powers on or resets:
 *    1. The CPU reads address 0x08000000 -> the initial stack pointer.
 *    2. The CPU reads address 0x08000004 -> the address of Reset_Handler.
 *    3. It jumps to Reset_Handler, which:
 *         - copies initial values of global variables from flash to RAM
 *         - zeroes the rest of the global variables
 *         - turns on the floating-point unit
 *         - calls main()
 *
 *  The "vector table" below is that list of addresses at 0x08000000.
 *  Entries after the first 16 are interrupt handlers (e.g. TIM2_IRQHandler).
 *  If you write a function with one of those names, it is called
 *  automatically when that interrupt fires.
 * =============================================================================
 */
#include <stdint.h>

/* Symbols created by the linker script (linker/stm32f446re.ld). */
extern uint32_t _estack;   /* top of RAM = initial stack pointer          */
extern uint32_t _sidata;   /* where .data's initial values sit in flash   */
extern uint32_t _sdata;    /* start of .data in RAM                        */
extern uint32_t _edata;    /* end of .data in RAM                          */
extern uint32_t _sbss;     /* start of .bss in RAM                         */
extern uint32_t _ebss;     /* end of .bss in RAM                           */

int main(void);

/* -----------------------------------------------------------------------------
 *  Default handler: if an interrupt fires that nobody wrote a handler for,
 *  or the CPU faults (bad pointer, etc.), we end up here and stop.
 *  In the debugger, if your program "freezes" here, something went wrong.
 * -------------------------------------------------------------------------- */
void Default_Handler(void)
{
    for (;;) {
        /* Stuck here? Check the call stack in the debugger. */
    }
}

void Reset_Handler(void);

/* "weak alias" = use Default_Handler unless you write your own function
 * with this exact name somewhere in your code. */
#define WEAK_DEFAULT __attribute__((weak, alias("Default_Handler")))

void NMI_Handler(void)             WEAK_DEFAULT;
void HardFault_Handler(void)       WEAK_DEFAULT;
void MemManage_Handler(void)       WEAK_DEFAULT;
void BusFault_Handler(void)        WEAK_DEFAULT;
void UsageFault_Handler(void)      WEAK_DEFAULT;
void SVC_Handler(void)             WEAK_DEFAULT;
void DebugMon_Handler(void)        WEAK_DEFAULT;
void PendSV_Handler(void)          WEAK_DEFAULT;
void SysTick_Handler(void)         WEAK_DEFAULT;

/* A few STM32 interrupts you might want later (RM0390 Table 38). */
void EXTI0_IRQHandler(void)        WEAK_DEFAULT;   /* IRQ 6  */
void TIM2_IRQHandler(void)         WEAK_DEFAULT;   /* IRQ 28 */
void TIM3_IRQHandler(void)         WEAK_DEFAULT;   /* IRQ 29 */
void USART2_IRQHandler(void)       WEAK_DEFAULT;   /* IRQ 38 */
void EXTI15_10_IRQHandler(void)    WEAK_DEFAULT;   /* IRQ 40 (PC13 button) */

#define IRQ(n)  (16 + (n))      /* STM32 interrupt n sits at table slot 16+n */
#define NUM_IRQS 97             /* STM32F446 has 97 interrupt lines          */

/* The vector table. The linker script places section ".isr_vector"
 * at the very start of flash (0x08000000). */
__attribute__((section(".isr_vector"), used))
void (* const g_vector_table[16 + NUM_IRQS])(void) = {
    /* Every interrupt slot defaults to Default_Handler... */
    [IRQ(0) ... IRQ(NUM_IRQS - 1)] = Default_Handler,

    /* ...then the core entries and the ones we named above. */
    [0]  = (void (*)(void))(&_estack),   /* initial stack pointer */
    [1]  = Reset_Handler,
    [2]  = NMI_Handler,
    [3]  = HardFault_Handler,
    [4]  = MemManage_Handler,
    [5]  = BusFault_Handler,
    [6]  = UsageFault_Handler,
    [11] = SVC_Handler,
    [12] = DebugMon_Handler,
    [14] = PendSV_Handler,
    [15] = SysTick_Handler,

    [IRQ(6)]  = EXTI0_IRQHandler,
    [IRQ(28)] = TIM2_IRQHandler,
    [IRQ(29)] = TIM3_IRQHandler,
    [IRQ(38)] = USART2_IRQHandler,
    [IRQ(40)] = EXTI15_10_IRQHandler,
};

/* Coprocessor Access Control Register (Cortex-M4 core). Bits 23:20 = 1111
 * gives full access to the floating-point unit. Without this, any float
 * math would crash the chip. */
#define SCB_CPACR (*(volatile uint32_t *)0xE000ED88U)

void Reset_Handler(void)
{
    /* 1. Copy initialized globals (int x = 5;) from flash into RAM. */
    uint32_t *src = &_sidata;
    for (uint32_t *dst = &_sdata; dst < &_edata; ) {
        *dst++ = *src++;
    }

    /* 2. Zero the uninitialized globals (int count;). C promises they're 0. */
    for (uint32_t *dst = &_sbss; dst < &_ebss; ) {
        *dst++ = 0U;
    }

    /* 3. Turn on the floating-point unit. */
    SCB_CPACR |= (0xFU << 20);
    __asm volatile ("dsb\n isb");

    /* 4. Run the program. */
    main();

    /* main() should loop forever. If it ever returns, stop here. */
    for (;;) { }
}
