#include "diagonal_cube.h"
#include <stdlib.h>
extern volatile uint16_t current_speed;

static size_layer_t size_layer;
static uint32_t time_start = 0;
//const uint32_t time_todo_diagonal = 150;

// Hàm trung gian đưa dữ liệu Hex vào buffer theo logic cũ của bạn
// layer_hex xác định những tầng (Z) nào sẽ sáng
// col_data[8] xác định hình dáng (X, Y) trên mỗi tầng đó
void Map_Diagonal_To_Buffer(uint8_t *col_data, uint8_t layer_hex) {
    for (int z = 0; z < 8; z++) {
        // Kiểm tra xem tầng Z này có được phép sáng không
        if (layer_hex & (1 << z)) {
            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {
                    // Kiểm tra bit tương ứng trong mảng cột
                    if (col_data[y] & (1 << x)) {
                        Set_Voxel(x, y, z, 1);
                    }
                }
            }
        }
    }
}

void DiagonalCube_Set_State() {
    size_layer = SMALLEST;
    time_start = HAL_GetTick();
}

void DiagonalCube_Handle() {
    uint8_t temp_columns[8] = {0};
    uint8_t temp_layer = 0;

    // Logic Switch-case giữ nguyên của bạn
    switch (size_layer) {
        case SMALLEST:
            if (HAL_GetTick() - time_start >= current_speed) {
                temp_layer = 0x18; // Tầng 3 và 4
                for (int i = 3; i < 5; i++) temp_columns[i] = 0x18;

                Clear_Cube();
                Map_Diagonal_To_Buffer(temp_columns, temp_layer);

                size_layer = MEDIUM;
                time_start = HAL_GetTick();
            }
            break;

        case MEDIUM:
            if (HAL_GetTick() - time_start >= current_speed) {
                temp_layer = 0x24; // Tầng 2 và 5
                for (int i = 2; i < 6; i++) temp_columns[i] = 0x3c;

                Clear_Cube();
                Map_Diagonal_To_Buffer(temp_columns, temp_layer);

                size_layer = LARGE;
                time_start = HAL_GetTick();
            }
            break;

        case LARGE:
            if (HAL_GetTick() - time_start >= current_speed) {
                temp_layer = 0x42; // Tầng 1 và 6
                for (int i = 1; i < 7; i++) temp_columns[i] = 0x7e;

                Clear_Cube();
                Map_Diagonal_To_Buffer(temp_columns, temp_layer);

                size_layer = LARGEST;
                time_start = HAL_GetTick();
            }
            break;

        case LARGEST:
            if (HAL_GetTick() - time_start >= current_speed) {
                temp_layer = 0x81; // Tầng 0 và 7
                for (int i = 0; i < 8; i++) temp_columns[i] = 0xff;

                Clear_Cube();
                Map_Diagonal_To_Buffer(temp_columns, temp_layer);

                size_layer = SMALLEST;
                time_start = HAL_GetTick();
            }
            break;
    }
}
