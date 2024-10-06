#include "ch.h"
#include "hal.h"
#include "adc.h"

static THD_WORKING_AREA(waOutThread, 128);
static THD_FUNCTION(OutThread, p)
{
    (void)p;
    chRegSetThreadName("Out_Thread");

    while (true)
    {
        chThdSleepMilliseconds(10);
    }
}

void startOutThread(void)
{
    chThdCreateStatic(waOutThread, sizeof(waOutThread), NORMALPRIO, OutThread, NULL);
}