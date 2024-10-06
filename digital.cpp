#include "ch.h"
#include "hal.h"
#include "adc.h"

static THD_WORKING_AREA(waDigitalThread, 128);
static THD_FUNCTION(DigitalThread, p)
{
    (void)p;
    chRegSetThreadName("Digital_Thread");

    while (true)
    {
        chThdSleepMilliseconds(10);
    }
}

void startDigitalThread(void)
{
    chThdCreateStatic(waDigitalThread, sizeof(waDigitalThread), NORMALPRIO + 2, DigitalThread, NULL);
}