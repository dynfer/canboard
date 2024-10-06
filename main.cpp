#include "ch.h"
#include "hal.h"
#include "Can.h"
#include "Adc.h"
#include "Out.h"
#include "Digital.h"

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

    sdStart(&SD1, NULL);

    startAdcThread();
    startDigitalThread();
    startOutThread();
    startCanThread();

    while (true) {
        chThdSleepMilliseconds(1000);
    }
}
