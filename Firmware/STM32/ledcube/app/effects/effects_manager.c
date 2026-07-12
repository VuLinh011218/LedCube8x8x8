#include <around_edge_cube.h>
#include <count_down_cube.h>
#include <diagonal_cube.h>
#include <effects_manager.h>
#include <grow_shrink_cube.h>
#include <heart_effect.h>
#include <plane_cube.h>
#include <random_rain_cube.h>
#include <spiral_cube.h>

static CubeMode_t current_mode = MODE_OFF;
static uint32_t random_timer = 0;

extern volatile uint16_t current_speed;

void Effects_Manager_Init(void) {
	Clear_Cube();

	current_mode = MODE_OFF;

	// If using Random mode
	srand(HAL_GetTick());

	random_timer = 0;
}

void Effects_Manager_SetMode(CubeMode_t new_mode) {
	if (new_mode >= MODE_MAX)
		return;

	current_mode = new_mode;

	// Each time the mode changes, call the corresponding Set_State function to reset the variable.
	switch (current_mode) {
	case MODE_EDGE:
		break;
	case MODE_HEART:
		break;
	case MODE_COUNT:
		CountDownCube_Set_State();
		break;
	case MODE_DIAGONAL:
		DiagonalCube_Set_State();
		break;
	case MODE_GROW:
		GrowShrinkCube_Set_State();
		break;
	case MODE_PLANE:
		PlaneCube_Set_State();
		break;
	case MODE_RAIN:
		RandomRainCube_Set_State();
		break;
	case MODE_SPIRAL:
		SpiralCube_Set_State();
		break;
	case MODE_ADC:
		break;
	default:
		Clear_Cube();
		break;
	}
}

void Effects_Manager_Run(void) {
	switch (current_mode) {
	case MODE_EDGE:
		Effect_Around_Edge(current_speed, 1);
		break;
	case MODE_HEART:
		Effect_Heart_Final(current_speed);
		break;
	case MODE_COUNT:
		CountDownCube_Handle();
		break;
	case MODE_DIAGONAL:
		DiagonalCube_Handle();
		break;
	case MODE_GROW:
		GrowShrinkCube_Handle();
		break;
	case MODE_PLANE:
		PlaneCube_Handle();
		break;
	case MODE_RAIN:
		RandomRainCube_Handle(10);
		break;
	case MODE_SPIRAL:
		SpiralCube_Handle();
		break;
	case MODE_ADC:
		break;
	case MODE_OFF:
		Clear_Cube();
		break;
	default:
		break;
	}
}

