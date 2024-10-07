#include "Can.h"
#include "ch.h"
#include "hal.h"
#include "hal_can_lld.h"
#include "out.h"
#include "digital.h"
#include "spi.h"
#include "adc.h"

Can canVr;
CANRxFrame canRxMsg;
CANTxFrame canTxMsg1;
CANTxFrame canTxMsg2;
CANTxFrame canTxMsg3;
CANTxFrame canTxMsg4;

MUTEX_DECL(digitalOutputsMutex);
MUTEX_DECL(temperatureMutex);
MUTEX_DECL(digitalInputsMutex);
MUTEX_DECL(adcSampleMutex);

static const CANConfig cancfg =
    {
        CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
        /*
         For 72MHz http://www.bittiming.can-wiki.info/ gives us Pre-scaler=9, Seq 1=13 and Seq 2=2. Subtract '1' for register values
        */
        CAN_BTR_SJW(0) | CAN_BTR_BRP(8) | CAN_BTR_TS1(12) | CAN_BTR_TS2(1)

};

static const CANFilter filters[1] = {
    {
        .filter = 0,
        .mode = 0,                                         // Identifier mask mode (0 for mask mode)
        .scale = 1,                                        // 32-bit scale (1 for 32-bit)
        .assignment = 0,                                   // Assign to FIFO0
        .register1 = ((uint32_t)canVr.rxID << 21),         // ID 0x666 shifted as unsigned 32-bit value
        .register2 = ((uint32_t)0x7FFU << 21) | (1U << 2), // Standard ID mask, reject extended frames
    },
};

static THD_WORKING_AREA(waCanRxThread, 128);
static THD_FUNCTION(CanRxThread, p)
{
    (void)p;
    chRegSetThreadName("can_RX_Thread");

    while (true)
    {
        msg_t msg = canReceive(&CAND1, CAN_ANY_MAILBOX, &canRxMsg, TIME_INFINITE);
        if (msg != MSG_OK)
            continue;
        chMtxLock(&digitalOutputsMutex);
        if (canRxMsg.DLC > 0)
        {
            if (canRxMsg.data8[0])
            {
                uint8_t bits = canRxMsg.data8[0] & 0x0F;
                for (uint8_t i = 0; i < 4; i++)
                {
                    if (bits & (1 << i)) // Check if bit 'i' is set
                    {
                        USTAW_BIT(digitalOutputs, i);
                    }
                    else
                    {
                        CZYSC_BIT(digitalOutputs, i);
                    }
                }
            }
        }
        chMtxUnlock(&digitalOutputsMutex);
        chThdSleepMilliseconds(100);
    }
}

static THD_WORKING_AREA(waCanTxThread, 128);
static THD_FUNCTION(CanTxThread, p)
{
    (void)p;
    chRegSetThreadName("can_TX_Thread");
    canTxMsg1.IDE = CAN_IDE_STD;
    canTxMsg1.SID = canVr.txID1;
    canTxMsg1.RTR = CAN_RTR_DATA;
    canTxMsg1.DLC = 5;
    canTxMsg2.IDE = CAN_IDE_STD;
    canTxMsg2.SID = canVr.txID2;
    canTxMsg2.RTR = CAN_RTR_DATA;
    canTxMsg2.DLC = 8;
    canTxMsg3.IDE = CAN_IDE_STD;
    canTxMsg3.SID = canVr.txID3;
    canTxMsg3.RTR = CAN_RTR_DATA;
    canTxMsg3.DLC = 8;
    canTxMsg4.IDE = CAN_IDE_STD;
    canTxMsg4.SID = canVr.txID4;
    canTxMsg4.RTR = CAN_RTR_DATA;
    canTxMsg4.DLC = 4;
    while (true)
    {
        chMtxLock(&temperatureMutex);
        canTxMsg1.data16[0] = temperature1;
        canTxMsg1.data16[1] = temperature2;
        chMtxUnlock(&temperatureMutex);

        chMtxLock(&digitalInputsMutex);
        canTxMsg1.data8[4] = digitalInputs[0];
        chMtxUnlock(&digitalInputsMutex);

        chMtxLock(&adcSampleMutex);
        canTxMsg2.data16[0] = adc1_samples[0];
        canTxMsg2.data16[1] = adc1_samples[1];
        canTxMsg2.data16[2] = adc1_samples[2];
        canTxMsg2.data16[3] = adc1_samples[3];
        canTxMsg3.data16[4] = adc1_samples[4];
        canTxMsg3.data16[5] = adc1_samples[5];
        canTxMsg3.data16[6] = adc1_samples[6];
        canTxMsg3.data16[7] = adc1_samples[7];
        canTxMsg4.data16[8] = adc1_samples[8];
        canTxMsg4.data16[9] = adc1_samples[9];
        chMtxUnlock(&adcSampleMutex);

        canTransmit(&CAND1, CAN_ANY_MAILBOX, &canTxMsg1, TIME_INFINITE);
        canTransmit(&CAND1, CAN_ANY_MAILBOX, &canTxMsg2, TIME_INFINITE);
        canTransmit(&CAND1, CAN_ANY_MAILBOX, &canTxMsg3, TIME_INFINITE);
        canTransmit(&CAND1, CAN_ANY_MAILBOX, &canTxMsg4, TIME_INFINITE);

        chThdSleepMilliseconds(100);
    }
}

void startCanThread(void)
{
    canSTM32SetFilters(&CAND1, 0, 1, &filters[0]);
    canStart(&CAND1, &cancfg);
    chThdCreateStatic(waCanRxThread, sizeof(waCanRxThread), NORMALPRIO + 32, CanRxThread, NULL);
    chThdCreateStatic(waCanTxThread, sizeof(waCanTxThread), NORMALPRIO + 32, CanTxThread, NULL);
}