#ifndef OUT_H
#define OUT_H

#include <inttypes.h>
#include "ch.h"
#include "hal.h"

#define USTAW_BIT(array, index) (array[(index) / 8] |= (1 << ((index) % 8)))
#define CZYSC_BIT(array, index) (array[(index) / 8] &= ~(1 << ((index) % 8)))
#define ODCZYTAJ_BIT(array, index) ((array[(index) / 8] & (1 << ((index) % 8))) ? 1 : 0)

extern uint8_t digitalOutputs[(4 + 7) / 8]; // Declaration without initialization

void startOutThread(void);

#endif