#ifndef SPI_H
#define SPI_H

#include <inttypes.h>
#include "ch.h"
#include "hal.h"

extern uint16_t temperature1;
extern uint16_t temperature2;

void startSpiThread(void);

uint16_t max6675_read_temperature(uint8_t cs_pin);

#endif