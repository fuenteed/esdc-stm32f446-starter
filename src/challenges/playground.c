/*
 * =============================================================================
 *  PLAYGROUND — your own experiments
 * =============================================================================
 *  Pick the "env:playground" environment in the VS Code status bar
 *  (or run:  pio run -e playground -t upload ) and build whatever you like.
 *
 *  Ideas: Morse code your name on PA5, a dice roll on button press,
 *         a traffic light with a pedestrian button.
 * =============================================================================
 */
#include "esdc.h"

int main(void)
{
    RCC_AHB1ENR |= (1U << 0);          /* GPIOA clock on      */
    PIN_OUTPUT(GPIOA_MODER, 5);        /* PA5 = onboard LED   */

    while (1) {
        /* your code here */
        GPIOA_ODR ^= (1U << 5);
        delay_ms(1000);
    }
}
