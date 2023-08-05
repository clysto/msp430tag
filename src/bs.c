#include "bs.h"

#include <driverlib.h>
#include <stdlib.h>

void BS_init(uint8_t clockSource, uint32_t desiredClock) {
    EUSCI_B_SPI_initMasterParam param = {0};
    if (clockSource == BS_ACLK) {
        param.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_ACLK;
        param.clockSourceFrequency = CS_getACLK();
    } else {
        param.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
        param.clockSourceFrequency = CS_getSMCLK();
    }
    param.desiredSpiClock = desiredClock;
    param.msbFirst = EUSCI_B_SPI_MSB_FIRST;
    param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    param.spiMode = EUSCI_B_SPI_3PIN;
    EUSCI_B_SPI_initMaster(EUSCI_B0_BASE, &param);
    EUSCI_B_SPI_enable(EUSCI_B0_BASE);
    EUSCI_B_SPI_clearInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT);
    EUSCI_B_SPI_clearInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN6, GPIO_SECONDARY_MODULE_FUNCTION);
}

void BS_transmitData(uint8_t const* data, size_t size) {
    for (int i = 0; i < size; i++) {
        UCB0TXBUF = data[i];
        while (!(UCB0IFG & UCTXIFG))
            ;
        UCB0IFG &= ~UCTXIFG;
    }
}
