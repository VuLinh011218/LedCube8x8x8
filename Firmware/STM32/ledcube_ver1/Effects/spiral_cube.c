#include "spiral_cube.h"
#include "display_led.h"

extern volatile uint16_t current_speed;

// Biến trạng thái nội bộ
static rollSize_t rollSize = ROLL_BIGGEST;
static axis_t axisDirection = Y_AXIS_1;
static directRotate_t directRotate = CLOCKWISE;
static uint8_t x = 0, y = 0;
static const uint8_t z_all = 8; // Bật toàn bộ tầng
static uint32_t timeStart = 0;

// Nguyên mẫu các hàm xử lý nội bộ
static void RollBiggestCounterClockWise_Handle(void);
static void RollBiggerCounterClockWise_Handle(void);
static void RollMediumCounterClockWise_Handle(void);
static void RollSmallCounterClockWise_Handle(void);
static void RollBiggestClockWise_Handle(void);
static void RollBiggerClockWise_Handle(void);
static void RollMediumClockWise_Handle(void);
static void RollSmallClockWise_Handle(void);

void SpiralCube_Set_State() {
    Clear_Cube();
    x = 7;
    y = 0;
    rollSize = ROLL_BIGGEST;
    directRotate = CLOCKWISE;
    axisDirection = Y_AXIS_1;
    timeStart = HAL_GetTick();
}

void SpiralCube_Handle() {
    // Luôn hiển thị cột hiện tại
    DisplayLed(x, y, z_all);

    if (directRotate == COUNTER_CLOCKWISE) {
        switch (rollSize) {
            case ROLL_BIGGEST: RollBiggestCounterClockWise_Handle(); break;
            case ROLL_BIGGER:  RollBiggerCounterClockWise_Handle();  break;
            case ROLL_MEDIUM:  RollMediumCounterClockWise_Handle();  break;
            case ROLL_SMALL:   RollSmallCounterClockWise_Handle();   break;
        }
    } else {
        switch (rollSize) {
            case ROLL_BIGGEST: RollBiggestClockWise_Handle(); break;
            case ROLL_BIGGER:  RollBiggerClockWise_Handle();  break;
            case ROLL_MEDIUM:  RollMediumClockWise_Handle();  break;
            case ROLL_SMALL:   RollSmallClockWise_Handle();   break;
        }
    }
}

// --- LOGIC QUAY NGƯỢC CHIỀU KIM ĐỒNG HỒ ---
static void RollBiggestCounterClockWise_Handle() {
    if (HAL_GetTick() - timeStart >= current_speed) {
        timeStart = HAL_GetTick();
        switch (axisDirection) {
            case Y_AXIS_1: if (y == 7) axisDirection = X_AXIS_1; else y++; break;
            case X_AXIS_1: if (x == 7) axisDirection = Y_AXIS_2; else x++; break;
            case Y_AXIS_2: if (y == 0) axisDirection = X_AXIS_2; else y--; break;
            case X_AXIS_2: if (x == 1) { axisDirection = Y_AXIS_1; rollSize = ROLL_BIGGER; } else x--; break;
        }
    }
}

// (Các hàm RollBigger, Medium, Small cho CounterClockWise tương tự logic cũ của bạn...)
static void RollBiggerCounterClockWise_Handle() {
    if (HAL_GetTick() - timeStart >= current_speed) {
        timeStart = HAL_GetTick();
        switch (axisDirection) {
            case Y_AXIS_1: if (y == 6) axisDirection = X_AXIS_1; else y++; break;
            case X_AXIS_1: if (x == 6) axisDirection = Y_AXIS_2; else x++; break;
            case Y_AXIS_2: if (y == 1) axisDirection = X_AXIS_2; else y--; break;
            case X_AXIS_2: if (x == 2) { axisDirection = Y_AXIS_1; rollSize = ROLL_MEDIUM; } else x--; break;
        }
    }
}

static void RollMediumCounterClockWise_Handle() {
    if (HAL_GetTick() - timeStart >= current_speed) {
        timeStart = HAL_GetTick();
        switch (axisDirection) {
            case Y_AXIS_1: if (y == 5) axisDirection = X_AXIS_1; else y++; break;
            case X_AXIS_1: if (x == 5) axisDirection = Y_AXIS_2; else x++; break;
            case Y_AXIS_2: if (y == 2) axisDirection = X_AXIS_2; else y--; break;
            case X_AXIS_2: if (x == 3) { axisDirection = Y_AXIS_1; rollSize = ROLL_SMALL; } else x--; break;
        }
    }
}

static void RollSmallCounterClockWise_Handle() {
    if (HAL_GetTick() - timeStart >= current_speed) {
        timeStart = HAL_GetTick();
        switch (axisDirection) {
            case Y_AXIS_1: if (y == 4) axisDirection = X_AXIS_1; else y++; break;
            case X_AXIS_1: if (x == 4) axisDirection = Y_AXIS_2; else x++; break;
            case Y_AXIS_2: if (y == 3) { directRotate = CLOCKWISE; axisDirection = Y_AXIS_1; rollSize = ROLL_BIGGEST; y = 0; x = 7; } else y--; break;
        }
    }
}

// --- LOGIC QUAY THUẬN CHIỀU KIM ĐỒNG HỒ ---
static void RollBiggestClockWise_Handle() {
    if (HAL_GetTick() - timeStart >= current_speed) {
        timeStart = HAL_GetTick();
        switch (axisDirection) {
            case Y_AXIS_1: if (y == 7) axisDirection = X_AXIS_1; else y++; break;
            case X_AXIS_1: if (x == 0) axisDirection = Y_AXIS_2; else x--; break;
            case Y_AXIS_2: if (y == 0) axisDirection = X_AXIS_2; else y--; break;
            case X_AXIS_2: if (x == 6) { rollSize = ROLL_BIGGER; axisDirection = Y_AXIS_1; } else x++; break;
        }
    }
}

static void RollBiggerClockWise_Handle() {
    if (HAL_GetTick() - timeStart >= current_speed) {
        timeStart = HAL_GetTick();
        switch (axisDirection) {
            case Y_AXIS_1: if (y == 6) axisDirection = X_AXIS_1; else y++; break;
            case X_AXIS_1: if (x == 1) axisDirection = Y_AXIS_2; else x--; break;
            case Y_AXIS_2: if (y == 1) axisDirection = X_AXIS_2; else y--; break;
            case X_AXIS_2: if (x == 5) { axisDirection = Y_AXIS_1; rollSize = ROLL_MEDIUM; } else x++; break;
        }
    }
}

static void RollMediumClockWise_Handle() {
    if (HAL_GetTick() - timeStart >= current_speed) {
        timeStart = HAL_GetTick();
        switch (axisDirection) {
            case Y_AXIS_1: if (y == 5) axisDirection = X_AXIS_1; else y++; break;
            case X_AXIS_1: if (x == 2) axisDirection = Y_AXIS_2; else x--; break;
            case Y_AXIS_2: if (y == 2) axisDirection = X_AXIS_2; else y--; break;
            case X_AXIS_2: if (x == 4) { axisDirection = Y_AXIS_1; rollSize = ROLL_SMALL; } else x++; break;
        }
    }
}

static void RollSmallClockWise_Handle() {
    if (HAL_GetTick() - timeStart >= current_speed) {
        timeStart = HAL_GetTick();
        switch (axisDirection) {
            case Y_AXIS_1: if (y == 4) axisDirection = X_AXIS_1; else y++; break;
            case X_AXIS_1: if (x == 3) axisDirection = Y_AXIS_2; else x--; break;
            case Y_AXIS_2: if (y == 3) { Clear_Cube(); axisDirection = Y_AXIS_1; rollSize = ROLL_BIGGEST; x = 0; y = 0; directRotate = COUNTER_CLOCKWISE; } else y--; break;
        }
    }
}
