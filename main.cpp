#include "ch.h"
#include "hal.h"
#include "Can.h"
#include "Adc.h"
#include "Out.h"
#include "spi.h"
#include "Digital.h"

Can canVar;

int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
    halInit();
    chSysInit();

    palSetPadMode(GPIOB, 9, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 8, PAL_MODE_INPUT_PULLDOWN);
    if (palReadPad(GPIOB, 9) && palReadPad(GPIOB, 8)) {
        canVar.rxID = 0x200;
        canVar.txID1 = 0x201;
        canVar.txID2 = 0x202;
        canVar.txID3 = 0x203;
        canVar.txID4 = 0x204;
    }
    else if(!palReadPad(GPIOB, 9) && palReadPad(GPIOB, 8)) {
        canVar.rxID = 0x300;
        canVar.txID1 = 0x301;
        canVar.txID2 = 0x302;
        canVar.txID3 = 0x303;
        canVar.txID4 = 0x304;
    }
    else if(palReadPad(GPIOB, 9) && !palReadPad(GPIOB, 8))  {
        canVar.rxID = 0x400;
        canVar.txID1 = 0x401;
        canVar.txID2 = 0x402;
        canVar.txID3 = 0x403;
        canVar.txID4 = 0x404;
    }
    else  {
        canVar.rxID = 0x500;
        canVar.txID1 = 0x501;
        canVar.txID2 = 0x502;
        canVar.txID3 = 0x503;
        canVar.txID4 = 0x504;
    }

    sdStart(&SD1, NULL);

    startAdcThread();
    startSpiThread();
    startDigitalThread();
    startOutThread();
    startCanThread();

    while (true) {
        chThdSleepMilliseconds(1000);
    }
}
