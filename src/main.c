#include <driverlib.h>
#include <stdio.h>
#include <string.h>

#include "bs.h"
#include "led.h"
#include "uart.h"

static void __attribute__((naked, section(".crt_0042"), used)) disable_watchdog(void) {
    WDTCTL = WDTPW | WDTHOLD;
}

int main(void) {
    // HFXTCLK: PJ.4, PJ.5
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    // SMCLK OUTPUT: P3.4
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN4, GPIO_SECONDARY_MODULE_FUNCTION);
    LED_init();
    // UCA0TXD P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);
    // UCA0RXD P2.1
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN1, GPIO_SECONDARY_MODULE_FUNCTION);
    PMM_unlockLPM5();

    // init serial port
    UART_init();
    LED_turnOnLED1();

    CS_setExternalClockSource(0, 16000000);
    CS_turnOnHFXT(CS_HFXT_DRIVE_16MHZ_24MHZ);
    // SMCLK = HFXTCLK / 2 = 8MHz
    CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_2);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    // baud rate: 1Mbps
    BS_init(BS_SMCLK, 1000000);

    uint8_t data[32];

    for (;;) {
        UART_transaction(data, sizeof(data));
        // read data from uart
        BS_transmitData(data, 32);
        __delay_cycles(8000);
    }

    __bis_SR_register(GIE);
    return 0;
}
