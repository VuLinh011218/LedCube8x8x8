#include "random_rain_cube.h"
#include "display_led.h"
#include <stdlib.h>

// Biến toàn cục từ Slider (Ví dụ: 20ms -> 500ms)
extern volatile uint16_t current_speed;

// BIẾN MỚI: Dùng để đếm thời gian riêng cho mưa (Không được ghi đè current_speed)
static uint32_t time_rain = 0;

typedef struct {
    uint8_t x;
    uint8_t y;
    int8_t z;
} RainDrop_t;

#define MAX_RAIN_DROPS 15
static RainDrop_t rain_drops[MAX_RAIN_DROPS];

void RandomRainCube_Set_State() {
    srand(HAL_GetTick());
    // Reset trạng thái hạt mưa
    for (int i = 0; i < MAX_RAIN_DROPS; i++) {
        rain_drops[i].z = -1;
    }

    // Khởi tạo mốc thời gian ban đầu
    time_rain = HAL_GetTick();
}

void RandomRainCube_Handle(uint8_t rain_times) {
    Clear_Cube();

    if (rain_times > MAX_RAIN_DROPS) rain_times = MAX_RAIN_DROPS;

    // --- SỬA Ở ĐÂY ---
    // 1. Dùng biến time_rain để tính toán thời gian trôi qua
    // 2. So sánh với (current_speed * 2) để làm CHẬM GẤP 2 LẦN
    // (Ví dụ Slider để 50ms -> Delay thực tế là 100ms)
    if (HAL_GetTick() - time_rain >= (current_speed * 2)) {

        // Cập nhật lại mốc thời gian
        time_rain = HAL_GetTick();

        // Logic rơi của hạt mưa (Giữ nguyên)
        for (uint8_t n = 0; n < rain_times; n++) {
            if (rain_drops[n].z < 0) {
                // Tạo hạt mưa mới ngẫu nhiên
                rain_drops[n].x = rand() % 8;
                rain_drops[n].y = rand() % 8;
                rain_drops[n].z = 7;
            } else {
                // Rơi xuống
                rain_drops[n].z--;
            }
        }
    }

    // Hiển thị (Giữ nguyên)
    for (uint8_t n = 0; n < rain_times; n++) {
        if (rain_drops[n].z >= 0) {
            Set_Voxel(rain_drops[n].x, rain_drops[n].y, (uint8_t)rain_drops[n].z, 1);
        }
    }
}
