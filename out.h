#ifndef OUT_H
#define OUT_H

#include <inttypes.h>
#include "ch.h"
#include "hal.h"

#define USTAW_BIT(array, index) (array[(index) / 8] |= (1 << ((index) % 8)))
#define CZYSC_BIT(array, index) (array[(index) / 8] &= ~(1 << ((index) % 8)))
#define ODCZYTAJ_BIT(array, index) ((array[(index) / 8] & (1 << ((index) % 8))) ? 1 : 0)


void startOutThread(void);

class Out
{
    private:

    public:
};

#endif