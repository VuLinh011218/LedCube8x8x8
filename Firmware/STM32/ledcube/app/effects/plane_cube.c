#include <plane_cube.h>
#include <stdlib.h>
extern volatile uint16_t current_speed;

static Plane_way_t way_state;
static uint8_t way;
static uint32_t timer = 0;


static uint8_t layer_plane;
static uint8_t column_plane[8];
static int i = 0;

static void Map_Plane_To_Buffer(void) {
    Clear_Cube();
    for (int z = 0; z < 8; z++) {
        if (layer_plane & (1 << z)) {
            for (int y = 0; y < 8; y++) {
                if (column_plane[y] != 0) {
                    for (int x = 0; x < 8; x++) {
                        if (column_plane[y] & (1 << x)) {
                            Set_Voxel(x, y, z, 1);
                        }
                    }
                }
            }
        }
    }
}

void PlaneCube_Set_State() {
    for (int j = 0; j < 8; j++) column_plane[j] = 0;
    i = 0;
    way = 1;
    layer_plane = 0;
    way_state = OX;
    timer = HAL_GetTick();
}

void PlaneCube_Handle() {
    switch (way_state) {
        case OX:
            layer_plane = 0xff;
            if (HAL_GetTick() - timer >= current_speed) {
                for (int j = 0; j < 8; j++) column_plane[j] = 0;
                column_plane[i] = 0xff;

                if (way) {
                    if (i >= 7) way = 0; else i++;
                } else {
                    if (i <= 0) {
                        way = 1;
                        way_state = OY;
                        i = 0;
                    } else i--;
                }
                timer = HAL_GetTick();
            }
            break;

        case OY:
            layer_plane = 0xff;
            if (column_plane[0] == 0 || column_plane[0] == 0xff) {
                for (int j = 0; j < 8; j++) column_plane[j] = 0x01;
            }
            if (HAL_GetTick() - timer >= current_speed) {
                if (way) {
                    for (int j = 0; j < 8; j++) column_plane[j] <<= 1;
                    if (column_plane[0] == 0x80) way = 0;
                } else {
                    for (int j = 0; j < 8; j++) column_plane[j] >>= 1;
                    if (column_plane[0] == 0x01) {
                        way = 1;
                        way_state = OZ;
                        layer_plane = 0;
                    }
                }
                timer = HAL_GetTick();
            }
            break;

        case OZ:
            for (int j = 0; j < 8; j++) column_plane[j] = 0xff;
            if (layer_plane == 0) layer_plane = 1;

            if (HAL_GetTick() - timer >= current_speed) {
                if (way) {
                    layer_plane <<= 1;
                    if (layer_plane == 0x80) way = 0;
                } else {
                    layer_plane >>= 1;
                    if (layer_plane == 0x01) {
                        way = 1;
                        way_state = OX;
                        i = 0;
                    }
                }
                timer = HAL_GetTick();
            }
            break;
    }
    Map_Plane_To_Buffer();
}
