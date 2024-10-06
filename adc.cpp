#include "ch.h"
#include "hal.h"
#include "adc.h"

static THD_WORKING_AREA(waAdcThread, 128);
static THD_FUNCTION(AdcThread, p)
{
    (void)p;
    chRegSetThreadName("Adc_Thread");

    while (true)
    {
        chThdSleepMilliseconds(10);
    }
}

void startAdcThread(void)
{
    chThdCreateStatic(waAdcThread, sizeof(waAdcThread), NORMALPRIO + 30, AdcThread, NULL);
}