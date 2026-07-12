#ifndef UART_TASK_H
#define UART_TASK_H

typedef enum
{
    UART_BAUD_115200 = 115200,
} UartBaudRate;

void uart_rx_task(void *arg);
void min_process_task(void *arg);
void rb_dump_task(void *arg);
void uart_init(void);

#endif
