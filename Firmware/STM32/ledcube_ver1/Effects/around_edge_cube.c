// Hiệu ứng chạy biên (Around Edge)
// Logic: Chạy một điểm sáng dọc theo 12 cạnh của khối lập phương
#include "around_edge_cube.h"
#include "display_led.h"
#include "effects_manager.h"
#include "min.h"

extern volatile CubeMode_t current_mode;
extern volatile uint16_t current_speed;

#define CHECK_EXIT if(current_mode != MODE_EDGE) return;

void Effect_Around_Edge(int delay_ms, int iterations) {
// Danh sách các đỉnh để tạo đường dẫn (Path)
// Điểm bắt đầu (0,0,0)
// Thứ tự di chuyển:
// Đáy: (0,0,0)->(7,0,0)->(7,7,0)->(0,7,0)->(0,0,0)
// Lên đỉnh: (0,0,0)->(0,0,7)
// Đỉnh: (0,0,7)->(7,0,7)->(7,7,7)->(0,7,7)->(0,0,7)
// ... Có nhiều biến thể, đây là biến thể "Snake" chạy quanh khung.
	CHECK_EXIT

	for (int iter = 0; iter < iterations; iter++) {
		// 1. Chạy cạnh đáy (Z=0)
		for (int i = 0; i < 7; i++) { Clear_Cube(); Set_Voxel(i, 0, 0, 1); Delay_Effect(current_speed); CHECK_EXIT} // X tăng
		for (int i = 0; i < 7; i++) { Clear_Cube(); Set_Voxel(7, i, 0, 1); Delay_Effect(current_speed); CHECK_EXIT} // Y tăng
		for (int i = 7; i > 0; i--) { Clear_Cube(); Set_Voxel(i, 7, 0, 1); Delay_Effect(current_speed); CHECK_EXIT} // X giảm
		for (int i = 7; i > 0; i--) { Clear_Cube(); Set_Voxel(0, i, 0, 1); Delay_Effect(current_speed); CHECK_EXIT} // Y giảm
		// 2. Leo lên 4 cột dọc (Hiệu ứng leo trôn ốc hoặc leo đồng loạt
		// Ở đây làm leo đồng loạt 4 góc cho đẹp
		for (int z = 0; z <= 7; z++) {
			Clear_Cube();
			Set_Voxel(0, 0, z, 1);
			Set_Voxel(7, 0, z, 1);
			Set_Voxel(7, 7, z, 1);
			Set_Voxel(0, 7, z, 1);
			Delay_Effect(current_speed);
			CHECK_EXIT
		}
		// 3. Chạy cạnh đỉnh (Z=7)
		for (int i = 0; i < 7; i++) { Clear_Cube(); Set_Voxel(i, 0, 7, 1); Delay_Effect(current_speed); CHECK_EXIT}
		for (int i = 0; i < 7; i++) { Clear_Cube(); Set_Voxel(7, i, 7, 1); Delay_Effect(current_speed); CHECK_EXIT}
		for (int i = 7; i > 0; i--) { Clear_Cube(); Set_Voxel(i, 7, 7, 1); Delay_Effect(current_speed); CHECK_EXIT}
		for (int i = 7; i > 0; i--) { Clear_Cube(); Set_Voxel(0, i, 7, 1); Delay_Effect(current_speed); CHECK_EXIT}
		// 4. Rơi xuống (Z giảm)
		for (int z = 7; z >= 0; z--) {
			Clear_Cube();
			Set_Voxel(0, 0, z, 1);
			Set_Voxel(7, 0, z, 1);
			Set_Voxel(7, 7, z, 1);
			Set_Voxel(0, 7, z, 1);
			Delay_Effect(current_speed);
			CHECK_EXIT
		}
	}
}
