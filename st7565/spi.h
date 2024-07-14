/*
 * spi.h
 *
 *  Created on: Jun 23, 2024
 *      Author: vlad
 */

#ifndef SPI_H_
#define SPI_H_

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"

#define PICO_SPI 0         // The pines are in the SPI1 area, so I don't know if I have to put here 0 or 1
#define PICO_SPI_SCK_PIN 2 // GP10 -> Pin #13 RP2040
#define PICO_SPI_TX_PIN 3  // GP12 --> PIN #15
#define PICO_SPI_RX_PIN 4  // GP11 --> PIN #14

void spiInit();
void spiWriteArr(uint8_t *data, uint16_t size);
void spiWriteChr(uint8_t data);
#endif /* SPI_H_ */
