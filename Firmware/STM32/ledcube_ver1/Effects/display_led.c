#include "display_led.h"

// Khai báo SPI từ main.c
extern SPI_HandleTypeDef hspi1;

// Biến nội bộ
static uint8_t ICcolumn[8];
static uint8_t IClayer;
static uint8_t dataOut[9];

void DisplayLed(uint8_t x, uint8_t y, uint8_t z)
{
    // 1. Tính toán vị trí Byte và Bit dựa trên tọa độ 3D
    uint8_t WhichByte = (((z << 6) + (y << 3) + x) >> 3);
    uint8_t WhichBit  = ((z << 6) + (y << 3) + x) - (WhichByte << 3);
    uint8_t WhichColumn = WhichByte - (z * 8);

    // 2. Xác định tầng (Layer) được bật
    if (z == 8) IClayer = 0xff; // Bật tất cả các tầng
    else IClayer = 0x01 << z;   // Chỉ bật tầng thứ z

    // 3. Xác định cột (Column) được bật
    ICcolumn[WhichColumn] = 0x01 << WhichBit;

    // 4. Chuẩn bị mảng dữ liệu 9 byte để gửi qua SPI
    // Byte đầu tiên là Layer, 8 byte sau là dữ liệu của 64 cột
    dataOut[0] = IClayer;
    for (uint8_t i = 1; i < 9; i++)
    {
        dataOut[i] = ICcolumn[i-1];
    }

    // 5. Xuất dữ liệu ra 74HC595
    HAL_GPIO_WritePin(LATCH_PIN_GPIO_Port, LATCH_PIN_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, dataOut, 9, 100);
    HAL_GPIO_WritePin(LATCH_PIN_GPIO_Port, LATCH_PIN_Pin, GPIO_PIN_SET);

    // 6. Xóa trạng thái để chuẩn bị cho điểm LED tiếp theo
    IClayer = 0;
    ICcolumn[WhichColumn] = 0;
}
