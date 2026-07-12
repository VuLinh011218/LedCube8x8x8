#include <count_down_cube.h>
#include <stdlib.h>
extern volatile uint16_t current_speed;

static int8_t   w_number             = 9;
static int8_t   i                    = 0;
static int8_t   direction            = 1;
static uint32_t time_start_countdown = 0;

// Bit order reversal function
uint8_t Reverse8(uint8_t b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

// col_hex: column data (8 bit), layer_hex: anable layer (8 bit)
// pos_y: current class position
void Map_Hex_To_Buffer(uint8_t col_hex, uint8_t layer_hex, int pos_y)
{
    uint8_t mirrored_col = Reverse8(col_hex);

    for (int layer = 0; layer < 8; layer++)
    {
        if (layer_hex & (1 << layer))
        {
            for (int col = 0; col < 8; col++)
            {
                if (mirrored_col & (1 << col))
                {
                    Set_Voxel(col, pos_y, layer, 1);
                }
            }
        }
    }
}

void DigitNine(int i)
{
    Map_Hex_To_Buffer(0x7e, 0x81, i);
    Map_Hex_To_Buffer(0xc3, 0x60, i);
    Map_Hex_To_Buffer(0x03, 0x0c, i);
    Map_Hex_To_Buffer(0xff, 0x02, i);
    Map_Hex_To_Buffer(0x7f, 0x10, i);
}

void DigitEight(int i)
{
    Map_Hex_To_Buffer(0xc3, 0x66, i);
    Map_Hex_To_Buffer(0x7e, 0x99, i);
}

void DigitSeven(int i)
{
    Map_Hex_To_Buffer(0xff, 0xc0, i);
    for (int j = 0; j < 6; j++)
    {
        Map_Hex_To_Buffer(0x03 << j, 0x20 >> j, i);
    }
}

void DigitSix(int i)
{
    Map_Hex_To_Buffer(0xff, 0x42, i);
    Map_Hex_To_Buffer(0xc3, 0x0c, i);
    Map_Hex_To_Buffer(0x7f, 0x80, i);
    Map_Hex_To_Buffer(0xc0, 0x20, i);
    Map_Hex_To_Buffer(0xfe, 0x10, i);
    Map_Hex_To_Buffer(0x7e, 0x01, i);
}

void DigitFive(int i)
{
    Map_Hex_To_Buffer(0xff, 0xc0, i);
    Map_Hex_To_Buffer(0xc0, 0x20, i);
    Map_Hex_To_Buffer(0xfe, 0x10, i);
    Map_Hex_To_Buffer(0x03, 0x0c, i);
    Map_Hex_To_Buffer(0x83, 0x02, i);
    Map_Hex_To_Buffer(0x7e, 0x01, i);
}

void DigitFour(int i)
{
    Map_Hex_To_Buffer(0x06, 0xf3, i);
    Map_Hex_To_Buffer(0xff, 0x0c, i);
    for (int j = 0; j < 4; j++)
    {
        Map_Hex_To_Buffer(0x08 << j, 0x80 >> j, i);
    }
}

void DigitThree(int i)
{
    Map_Hex_To_Buffer(0x7e, 0x81, i);
    Map_Hex_To_Buffer(0xe7, 0x42, i);
    Map_Hex_To_Buffer(0xc3, 0x24, i);
    Map_Hex_To_Buffer(0x06, 0x18, i);
}

void DigitTwo(int i)
{
    Map_Hex_To_Buffer(0x7e, 0x80, i);
    Map_Hex_To_Buffer(0xc3, 0x40, i);
    Map_Hex_To_Buffer(0x83, 0x20, i);
    Map_Hex_To_Buffer(0x0e, 0x10, i);
    Map_Hex_To_Buffer(0x1c, 0x08, i);
    Map_Hex_To_Buffer(0x70, 0x04, i);
    Map_Hex_To_Buffer(0xff, 0x03, i);
}

void DigitOne(int i)
{
    Map_Hex_To_Buffer(0xff, 0x01, i);
    Map_Hex_To_Buffer(0x98, 0x08, i);
    Map_Hex_To_Buffer(0xd8, 0x10, i);
    Map_Hex_To_Buffer(0x78, 0x20, i);
    Map_Hex_To_Buffer(0x38, 0x40, i);
    Map_Hex_To_Buffer(0x18, 0x86, i);
}

void DigitZero(int i)
{
    Map_Hex_To_Buffer(0x3c, 0x81, i);
    Map_Hex_To_Buffer(0x66, 0x42, i);
    Map_Hex_To_Buffer(0xc3, 0x3c, i);
}

void CountDownCube_Set_State()
{
    w_number             = 9;
    i                    = 0;
    direction            = 1;
    time_start_countdown = HAL_GetTick();
}

void CountDownCube_Handle()
{
    Clear_Cube();

    switch (w_number)
    {
        case 9:
            DigitNine(i);
            break;
        case 8:
            DigitEight(i);
            break;
        case 7:
            DigitSeven(i);
            break;
        case 6:
            DigitSix(i);
            break;
        case 5:
            DigitFive(i);
            break;
        case 4:
            DigitFour(i);
            break;
        case 3:
            DigitThree(i);
            break;
        case 2:
            DigitTwo(i);
            break;
        case 1:
            DigitOne(i);
            break;
        case 0:
            DigitZero(i);
            break;
    }

    if (HAL_GetTick() - time_start_countdown >= current_speed)
    {
        if (direction == 1 && (i < 7))
        {
            i++;
        }
        if (i >= 7)
        {
            direction = 0;
        }
        if (direction == 0 && (i > 0))
        {
            i--;
        }
        if (i == 0)
        {
            direction = 1;
            w_number--;
            if (w_number < 0)
            {
                w_number = 9;
            }
        }
        time_start_countdown = HAL_GetTick();
    }
}
