#include "Can.h"
#include "ch.h"
#include "hal.h"
#include "hal_can_lld.h"


CANRxFrame canRxMsg;
CANTxFrame canTxMsg;

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
        .register1 = ((uint32_t)0x700U << 21),             // ID 0x666 shifted as unsigned 32-bit value
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
        chThdSleepMilliseconds(10);
    }
}

static THD_WORKING_AREA(waCanTxThread, 128);
static THD_FUNCTION(CanTxThread, p)
{
    (void)p;
    chRegSetThreadName("can_TX_Thread");

    while (true)
    {
        canTransmit(&CAND1, CAN_ANY_MAILBOX, &canTxMsg, TIME_MS2I(100));
        chThdSleepMilliseconds(10);
    }
}

void startCanThread(void)
{
    canSTM32SetFilters(&CAND1, 0, 1, &filters[0]);
    canStart(&CAND1, &cancfg);
    chThdCreateStatic(waCanRxThread, sizeof(waCanRxThread), NORMALPRIO + 32, CanRxThread, NULL);
    chThdCreateStatic(waCanTxThread, sizeof(waCanTxThread), NORMALPRIO + 32, CanTxThread, NULL);
}