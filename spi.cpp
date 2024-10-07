#include "ch.h"
#include "hal.h"
#include "spi.h"
#include "hal_spi_lld.h"
#include "Can.h"

static SPIConfig spi2_cfg = {
    false,
    NULL,
    GPIOB,
    11,
    ((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |
			SPI_CR1_CPHA |
			0,
    0
};

// Global variables to store the temperatures
uint16_t temperature1 = 0;
uint16_t temperature2 = 0;

// Function to read temperature from MAX6675
uint16_t max6675_read_temperature(uint8_t cs_pin) {
  uint16_t value = 0;

  spiAcquireBus(&SPID2);              // Acquire ownership of the bus
  spi2_cfg.ssport = GPIOB;            // Set port
  spi2_cfg.sspad = cs_pin;            // Set CS pin
  spiStart(&SPID2, &spi2_cfg);        // Start the SPI configuration
  spiSelect(&SPID2);                  // Select the SPI device

  uint8_t rxbuf[2] = {0};
  spiReceive(&SPID2, 2, rxbuf);       // Read 2 bytes from MAX6675
  spiUnselect(&SPID2);                // Unselect the SPI device
  spiStop(&SPID2);
  spiReleaseBus(&SPID2);              // Release ownership of the bus

  value = (rxbuf[0] << 8) | rxbuf[1]; // Combine high and low byte

  // Check if thermocouple is connected
  if (value & 0x4) {
    return 0xFFFF; // Thermocouple not connected
  }

  // Extract temperature data (12-bit resolution)
  value >>= 3;

  return value;
}

static THD_WORKING_AREA(waSpiThread, 128);
static THD_FUNCTION(SpiThread, p)
{
    (void)p;
    chRegSetThreadName("Spi_Thread");

    while (true)
    {
        chMtxLock(&temperatureMutex);
        temperature1 = max6675_read_temperature(11); // Read temperature from sensor 1 (CS pin B12)
        temperature2 = max6675_read_temperature(12); // Read temperature from sensor 2 (CS pin B13)
        chMtxUnlock(&temperatureMutex);
        chThdSleepMilliseconds(250);
    }
}

void startSpiThread(void)
{
    spiStart(&SPID2, &spi2_cfg);
    chThdCreateStatic(waSpiThread, sizeof(waSpiThread), NORMALPRIO, SpiThread, NULL);
}