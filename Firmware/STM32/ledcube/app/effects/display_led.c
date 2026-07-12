#include <display_led.h>

extern SPI_HandleTypeDef hspi1;

static uint8_t ICcolumn[8];
static uint8_t IClayer;
static uint8_t dataOut[9];

void DisplayLed(uint8_t x, uint8_t y, uint8_t z) {
	// Calculate byte and bit positions based on 3D coordinates.
	uint8_t WhichByte = (((z << 6) + (y << 3) + x) >> 3);
	uint8_t WhichBit = ((z << 6) + (y << 3) + x) - (WhichByte << 3);
	uint8_t WhichColumn = WhichByte - (z * 8);

	// Identify the layer that is turned on
	if (z == 8)
		IClayer = 0xff;
	else
		IClayer = 0x01 << z;

	// Identify the column that is turned on.
	ICcolumn[WhichColumn] = 0x01 << WhichBit;

	// The first byte is the Layer, the following 8 bytes contain the data for the 64 columns.
	dataOut[0] = IClayer;
	for (uint8_t i = 1; i < 9; i++) {
		dataOut[i] = ICcolumn[i - 1];
	}

	// Output data to 74HC595
	HAL_GPIO_WritePin(LATCH_PIN_GPIO_Port, LATCH_PIN_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, dataOut, 9, 100);
	HAL_GPIO_WritePin(LATCH_PIN_GPIO_Port, LATCH_PIN_Pin, GPIO_PIN_SET);

	IClayer = 0;
	ICcolumn[WhichColumn] = 0;
}
