#ifndef BS_H
#define BS_H

#include <stdint.h>
#include <stdlib.h>

#define BS_ACLK  0x00
#define BS_SMCLK 0x01

/**
 * @brief Initialize the Backscatte module.
 * The ACLK clock source is used by default.
 *
 * @param clockSource is the clock source to use.
 *        Valid values are:
 *        - \b BS_SMCLK
 *        - \b BS_ACLK
 * @param desiredClock is the desired clock speed.
 */
void BS_init(uint8_t clockSource, uint32_t desiredClock);

/**
 * @brief Transmit data using backscatter.
 *
 * @param data is the data to transmit.
 * @param size is the size of the data to transmit.
 */
void BS_transmitData(uint8_t const* data, size_t size);

#endif
