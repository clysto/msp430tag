#include <driverlib.h>
#include <stdlib.h>
#include <string.h>

void UART_init() {
    /// 9600 bps
    // paramaters caculate using
    // https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 52;
    param.firstModReg = 1;
    param.secondModReg = 73;
    param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    EUSCI_A_UART_init(EUSCI_A0_BASE, &param);
    EUSCI_A_UART_enable(EUSCI_A0_BASE);
}

size_t UART_transaction(uint8_t *buf, uint32_t size) {
    // query format 1byte operation code + 4byte size
    // response format 4byte actual size + N byte data
    uint8_t query[5] = {0};
    // READ operation
    query[0] = 0xFF;
    memcpy(query + 1, &size, 4);
    // send query
    for (int i = 0; i < 5; i++) {
        EUSCI_A_UART_transmitData(EUSCI_A0_BASE, query[i]);
    }
    uint32_t actualSize = 0;
    for (int i = 0; i < 4; i++) {
        actualSize |= EUSCI_A_UART_receiveData(EUSCI_A0_BASE) << (i * 8);
    }
    // receive data
    for (int i = 0; i < actualSize; i++) {
        buf[i] = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
    }
    return actualSize;
}