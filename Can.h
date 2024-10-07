#ifndef CAN_H
#define CAN_H

#include <inttypes.h>
#include "ch.h"
#include "hal.h"

#define USTAW_BIT(array, index) (array[(index) / 8] |= (1 << ((index) % 8)))
#define CZYSC_BIT(array, index) (array[(index) / 8] &= ~(1 << ((index) % 8)))
#define ODCZYTAJ_BIT(array, index) ((array[(index) / 8] & (1 << ((index) % 8))) ? 1 : 0)

extern CANRxFrame canRxMsg;
extern CANTxFrame canTxMsg1;
extern CANTxFrame canTxMsg2;
extern CANTxFrame canTxMsg3;
extern CANTxFrame canTxMsg4;


extern mutex_t digitalOutputsMutex;
extern mutex_t temperatureMutex;
extern mutex_t digitalInputsMutex;
extern mutex_t adcSampleMutex;

void startCanThread(void);

class Can
{
    private:

    public:
    uint16_t rxID;
    uint16_t txID1;
    uint16_t txID2;
    uint16_t txID3;
    uint16_t txID4;
};

#endif