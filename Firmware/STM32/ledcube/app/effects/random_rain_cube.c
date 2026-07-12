#include <display_led.h>
#include <random_rain_cube.h>
#include <stdlib.h>

extern volatile uint16_t current_speed;

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
    for (int i = 0; i < MAX_RAIN_DROPS; i++) {
        rain_drops[i].z = -1;
    }

    time_rain = HAL_GetTick();
}

void RandomRainCube_Handle(uint8_t rain_times) {
    Clear_Cube();

    if (rain_times > MAX_RAIN_DROPS) rain_times = MAX_RAIN_DROPS;

    if (HAL_GetTick() - time_rain >= (current_speed * 2)) {

        time_rain = HAL_GetTick();

        for (uint8_t n = 0; n < rain_times; n++) {
            if (rain_drops[n].z < 0) {
                rain_drops[n].x = rand() % 8;
                rain_drops[n].y = rand() % 8;
                rain_drops[n].z = 7;
            } else {
                rain_drops[n].z--;
            }
        }
    }

    for (uint8_t n = 0; n < rain_times; n++) {
        if (rain_drops[n].z >= 0) {
            Set_Voxel(rain_drops[n].x, rain_drops[n].y, (uint8_t)rain_drops[n].z, 1);
        }
    }
}
