#include <driverlib.h>
#include <string.h>

#include "bs.h"
#include "led.h"

static void __attribute__((naked, section(".crt_0042"), used)) disable_watchdog(void) {
    WDTCTL = WDTPW | WDTHOLD;
}

uint8_t BUFFER[16 + 5] = {0x0A, 0xAA, 0xAA, 0xAD, 0x23};
char TEXT[] = "ABCDEFGHIJKLMNH";

int main(void) {
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN4 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN4, GPIO_SECONDARY_MODULE_FUNCTION);
    LED_init();
    PMM_unlockLPM5();

    LED_turnOnLED1();

    CS_setExternalClockSource(32768, 0);
    CS_turnOnLFXT(CS_LFXT_DRIVE_0);
    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    // baud rate: 1MHz
    BS_init(BS_SMCLK, 1e6);

    memcpy(BUFFER + 5, TEXT, 16);

    for (;;) {
        BS_transmitData(BUFFER, sizeof(BUFFER));
        __delay_cycles(8000);
    }

    __bis_SR_register(GIE);
    return 0;
}
