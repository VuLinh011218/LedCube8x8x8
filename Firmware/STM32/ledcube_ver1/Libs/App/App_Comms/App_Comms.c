#include "App_Comms.h"
#include "min.h"
#include <string.h> // <--- BẮT BUỘC để dùng strcmp, memcpy
#include <stdio.h>
#include <stdlib.h>
#include "effects_manager.h"
#include "main.h"

// Định nghĩa ID khớp với ESP32
#define MIN_ID_CMD_STR      0x03
#define MIN_ID_SPEED        0x04

// Biến toàn cục lưu trạng thái
volatile CubeMode_t current_mode = MODE_OFF;
volatile uint16_t current_speed = 500;

extern UART_HandleTypeDef huart2;

static struct min_context min_ctx;
static uint8_t rx_byte_u1;

// Biến theo dõi thời gian nhận Ping cuối cùng
static uint32_t last_ping_tick = 0;
// Biến trạng thái kết nối
static uint8_t is_connected = 0;

// === 2. HÀM KHỞI TẠO ===
void App_Comms_Init(void)
{
    min_init_context(&min_ctx, 0);
    // Bắt đầu nhận ngắt UART
    HAL_UART_Receive_IT(&huart2, &rx_byte_u1, 1);
}

// === 3. HÀM CHẠY LOGIC CHÍNH (Loop) ===
void App_Comms_Run(void)
{
	// Nếu quá 4000ms (4 giây) mà không nhận được PING từ Node-RED
	    if (is_connected && (HAL_GetTick() - last_ping_tick > 4000)) {
	        is_connected = 0;

	        // TẮT ĐÈN PC13 (PC13 thường tích cực thấp: 1 là Tắt, 0 là Sáng)
	        // Nếu mạch bạn khác thì đảo lại logic này
	        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	    }
}

// === 4. XỬ LÝ GÓI TIN NHẬN ĐƯỢC (Callback) ===
void min_application_handler(uint8_t min_id, uint8_t const *min_payload, uint8_t len_payload, uint8_t port)
{
    // --- XỬ LÝ CHẾ ĐỘ (MODE) ---
    if (min_id == MIN_ID_CMD_STR)
    {
        char cmd_str[32];

        // Copy an toàn
        if (len_payload >= 32) len_payload = 31;
        memcpy(cmd_str, min_payload, len_payload);
        cmd_str[len_payload] = '\0'; // Kết thúc chuỗi

        // ---  XỬ LÝ HEARTBEAT (PING) ---
        if (strncmp(cmd_str, "PING", 4) == 0) {
        	// Cập nhật thời gian
            last_ping_tick = HAL_GetTick();
            is_connected = 1;

            // BẬT ĐÈN PC13 (Báo hiệu đang kết nối)
            // PC13 tích cực thấp: 0 là Sáng
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

            // Gửi trả lời "PONG" lên Node-RED
            min_send_frame(&min_ctx, MIN_ID_CMD_STR, (uint8_t*)"PONG", 4);
        }

        // --- XỬ LÝ TỐC ĐỘ (SPEED) ---
        if (strncmp(cmd_str, "SPD:", 4) == 0) {
        	// Cắt bỏ 4 ký tự đầu "SPD:", lấy phần số còn lại
            int speed_val = atoi(cmd_str + 4);

            // Giới hạn tốc độ an toàn (10ms đến 1000ms)
            if(speed_val >= 10 && speed_val <= 1000) {
            	current_speed = (uint16_t)speed_val;
            	printf("STM32 Set Speed: %d ms\n", current_speed);
            }
        }

        // So sánh chuỗi để set Mode
        if (strcmp(cmd_str, "MODE_EDGE") == 0) {
            current_mode = MODE_EDGE;
            printf("STM32 Set Mode: EDGE\n");
        }
        else if (strcmp(cmd_str, "MODE_HEART") == 0) {
            current_mode = MODE_HEART;
            printf("STM32 Set Mode: HEART\n");
        }
        else if (strcmp(cmd_str, "MODE_COUNT") == 0) {
            current_mode = MODE_COUNT;
            printf("STM32 Set Mode: COUNT DOWN\n");
        }
        else if (strcmp(cmd_str, "MODE_DIAGONAL") == 0) {
            current_mode = MODE_DIAGONAL;
            printf("STM32 Set Mode: DIAGONAL\n");
        }
        else if (strcmp(cmd_str, "MODE_GROW") == 0) {
            current_mode = MODE_GROW;
            printf("STM32 Set Mode: GROW & SHRINK\n");
        }
        else if (strcmp(cmd_str, "MODE_RAIN") == 0) {
            current_mode = MODE_RAIN;
            printf("STM32 Set Mode: RAIN FALL\n");
        }
        else if (strcmp(cmd_str, "MODE_SPIRAL") == 0) {
            current_mode = MODE_SPIRAL;
            printf("STM32 Set Mode: SPIRAL WAVE\n");
        }
        else if (strcmp(cmd_str, "MODE_PLANE") == 0) {
            current_mode = MODE_PLANE;
            printf("STM32 Set Mode: AIRPLANE\n");
        }
        else if (strcmp(cmd_str, "MODE_OFF") == 0) {
            current_mode = MODE_OFF;
            printf("STM32 Set Mode: OFF\n");
        }
        else {
            printf("STM32 Unknown Command: %s\n", cmd_str);
        }
    }
}

// === 5. NGẮT UART ===
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        min_poll(&min_ctx, &rx_byte_u1, 1);
        HAL_UART_Receive_IT(&huart2, &rx_byte_u1, 1);
    }
}
