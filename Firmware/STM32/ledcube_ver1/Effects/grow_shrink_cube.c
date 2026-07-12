#include "grow_shrink_cube.h"
#include <stdbool.h>
#include <stdlib.h>
extern volatile uint16_t current_speed;

static SizeCube_t size_cube;
static uint32_t time_start = 0;
//const uint32_t time_todo2 = 200; // Tốc độ co giãn (ms)
static bool is_shrinking = false; // Biến điều khiển chiều co giãn

// Dữ liệu mã Hex cho các kích thước hộp đặc (2x2, 4x4, 6x6, 8x8)
static const uint8_t cube_data_hex[4] = {0x18, 0x3C, 0x7E, 0xFF};

// Hàm vẽ khối hộp đặc vào buffer dựa trên chỉ số index (0-3)
static void Map_Cube_To_Buffer(uint8_t index) {
    uint8_t data = cube_data_hex[index];

    for (int z = 0; z < 8; z++) {
        if (data & (1 << z)) { // Kiểm tra tầng Z
            for (int y = 0; y < 8; y++) {
                if (data & (1 << y)) { // Kiểm tra hàng Y
                    for (int x = 0; x < 8; x++) {
                        if (data & (1 << x)) { // Kiểm tra cột X
                            Set_Voxel(x, y, z, 1);
                        }
                    }
                }
            }
        }
    }
}

void GrowShrinkCube_Set_State() {
    size_cube = TWO_X_TWO_CUBE;
    is_shrinking = false;
    time_start = HAL_GetTick();
    Clear_Cube();
}

void GrowShrinkCube_Handle() {
    // 1. Logic tính toán trạng thái (Non-blocking)
    if (HAL_GetTick() - time_start >= current_speed) {
        time_start = HAL_GetTick();

        if (!is_shrinking) {
            // Đang nở ra
            if (size_cube < EIGHT_X_EIGHT_CUBE) {
                size_cube++;
            } else {
                is_shrinking = true; // Chạm đỉnh 8x8, bắt đầu thu lại
                size_cube--;
            }
        } else {
            // Đang thu lại
            if (size_cube > TWO_X_TWO_CUBE) {
                size_cube--;
            } else {
                is_shrinking = false; // Chạm đáy 2x2, bắt đầu nở ra
                size_cube++;
            }
        }
    }

    // 2. Hiển thị
    Clear_Cube();
    Map_Cube_To_Buffer((uint8_t)size_cube);
}
