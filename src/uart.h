#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdlib.h>

void UART_init();
size_t UART_transaction(uint8_t *buf, uint32_t size);

#endif