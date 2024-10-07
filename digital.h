#ifndef DIGITAL_H
#define DIGITAL_H

#include <inttypes.h>
#include "ch.h"
#include "hal.h"

#define USTAW_BIT(array, index) (array[(index) / 8] |= (1 << ((index) % 8)))
#define CZYSC_BIT(array, index) (array[(index) / 8] &= ~(1 << ((index) % 8)))
#define ODCZYTAJ_BIT(array, index) ((array[(index) / 8] & (1 << ((index) % 8))) ? 1 : 0)

extern uint8_t digitalInputs[(8 + 7) / 8]; // Declaration without initialization

void startDigitalThread(void);

#endif
