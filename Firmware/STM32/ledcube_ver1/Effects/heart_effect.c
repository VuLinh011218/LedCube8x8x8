#include "heart_effect.h"
#include <string.h>
#include "effects_manager.h"
#include "min.h"

extern volatile CubeMode_t current_mode;
extern volatile uint16_t current_speed;

// Thoát hiểm nếu đổi mode
#define CHECK_EXIT if(current_mode != MODE_HEART) return;

const uint8_t Heart_Points[14][2] = {
    {3, 1}, {2, 2}, {4, 2}, {1, 3}, {5, 3}, {0, 4}, {6, 4},
    {0, 5}, {3, 5}, {6, 5}, {1, 6}, {2, 6}, {4, 6}, {5, 6}
};

extern uint8_t cube_buffer[8][8];

// Hàm rơi: Cho rơi nhanh hơn nữa (chia 2)
void Drop_Group(int start_idx, int count, int layer_y, int speed) {
    CHECK_EXIT
    int target_z = Heart_Points[start_idx][1];

    for (int z = 7; z >= target_z; z--) {
        for (int k = 0; k < count; k++) {
            Set_Voxel(Heart_Points[start_idx + k][0], layer_y, z, 1);
        }

        // TỐC ĐỘ GẤP ĐÔI: Chia 2 thời gian chờ
        Delay_Effect(current_speed / 2);

        CHECK_EXIT
        if (z > target_z) {
            for (int k = 0; k < count; k++) {
                Set_Voxel(Heart_Points[start_idx + k][0], layer_y, z, 0);
            }
        }
    }
}

void Draw_Heart_Block_Offset(int off_x, int off_z) {
    for (int layer_y = 5; layer_y <= 7; layer_y++) {
        for (int i = 0; i < 14; i++) {
            int px = Heart_Points[i][0] + off_x;
            int pz = Heart_Points[i][1] + off_z;
            Set_Voxel(px, layer_y, pz, 1);
        }
    }
}

// --- HIỆU ỨNG CHÍNH ---
void Effect_Heart_Final(int speed) {
    CHECK_EXIT
    Clear_Cube();

    // --- GIAI ĐOẠN 1: MƯA RƠI ---
    // (Bên trong Drop_Group đã có /2 rồi nên sẽ rơi rất nhanh)
    Drop_Group(0, 1, 7, 0); CHECK_EXIT
    Drop_Group(1, 2, 7, 0); CHECK_EXIT
    Drop_Group(3, 2, 7, 0); CHECK_EXIT
    Drop_Group(5, 2, 7, 0); CHECK_EXIT
    Drop_Group(7, 3, 7, 0); CHECK_EXIT
    Drop_Group(10, 4, 7, 0); CHECK_EXIT

    // Nghỉ một chút
    // Cũ: *3 -> Mới: *1.5 (lấy *1 + nửa) hoặc *1 cho nhanh
    Delay_Effect(current_speed * 1);
    CHECK_EXIT

    // --- GIAI ĐOẠN 2: NHẤP NHÁY (Tim đập) ---
    for (int k = 0; k < 3; k++) {
        CHECK_EXIT
        Draw_Heart_Block_Offset(0, 0);

        // Cũ: *4 -> Mới: *2 (Sáng nhanh hơn)
        Delay_Effect(current_speed * 2);
        CHECK_EXIT

        Clear_Cube();

        // Cũ: *2 -> Mới: *1 (Tắt nhanh hơn)
        Delay_Effect(current_speed);
        CHECK_EXIT
    }

    Draw_Heart_Block_Offset(0, 0);
    Delay_Effect(current_speed);
    CHECK_EXIT

    // --- GIAI ĐOẠN 3: XOAY TRÒN ---
    int circle_path[8][2] = {
        {0, 1}, {1, 1}, {1, 0}, {1, -1},
        {0, -1}, {0, 0}, {0, 1}, {0, 0}
    };

    for (int round = 0; round < 3; round++) {
        for (int step = 0; step < 6; step++) {
            CHECK_EXIT
            Clear_Cube();
            int dx = circle_path[step][0];
            int dz = circle_path[step][1];

            Draw_Heart_Block_Offset(dx, dz);

            // Cũ: *2 -> Mới: *1 (Xoay nhanh gấp đôi)
            Delay_Effect(current_speed);
            CHECK_EXIT
        }
    }

    Clear_Cube();
    Draw_Heart_Block_Offset(0, 0);
    Delay_Effect(current_speed);
    CHECK_EXIT

    // --- GIAI ĐOẠN 4: LÙI VỀ SAU ---
    for (int shift = 1; shift < 10; shift++) {
        CHECK_EXIT
        Clear_Cube();
        for (int layer = 0; layer < 3; layer++) {
            int current_y = (7 - layer) - shift;
            if (current_y >= 0) {
                for (int i = 0; i < 14; i++) {
                    Set_Voxel(Heart_Points[i][0], current_y, Heart_Points[i][1], 1);
                }
            }
        }
        // Cũ: *1 -> Mới: /2 (Lùi nhanh như gió)
        Delay_Effect(current_speed / 2);
        CHECK_EXIT
    }

    // Kết thúc nghỉ ngơi
    Clear_Cube();
    // Cũ: *5 -> Mới: *2 (Nghỉ ngắn thôi để còn lặp lại)
    Delay_Effect(current_speed * 2);
    CHECK_EXIT
}
