#include <grow_shrink_cube.h>
#include <stdbool.h>
#include <stdlib.h>
extern volatile uint16_t current_speed;

static SizeCube_t size_cube;
static uint32_t time_start = 0;
static bool is_shrinking = false;

static const uint8_t cube_data_hex[4] = { 0x18, 0x3C, 0x7E, 0xFF };

static void Map_Cube_To_Buffer(uint8_t index) {
	uint8_t data = cube_data_hex[index];

	for (int z = 0; z < 8; z++) {
		if (data & (1 << z)) { //
			for (int y = 0; y < 8; y++) {
				if (data & (1 << y)) {
					for (int x = 0; x < 8; x++) {
						if (data & (1 << x)) {
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
	if (HAL_GetTick() - time_start >= current_speed) {
		time_start = HAL_GetTick();

		if (!is_shrinking) {
			if (size_cube < EIGHT_X_EIGHT_CUBE) {
				size_cube++;
			} else {
				is_shrinking = true;
				size_cube--;
			}
		} else {
			if (size_cube > TWO_X_TWO_CUBE) {
				size_cube--;
			} else {
				is_shrinking = false;
				size_cube++;
			}
		}
	}

	Clear_Cube();
	Map_Cube_To_Buffer((uint8_t) size_cube);
}
