#ifndef MIN_PORT_H
#define MIN_PORT_H

#pragma once
#include <stdint.h>


uint32_t min_time_ms(void);
uint16_t min_tx_space(uint8_t port);
void min_tx_finished(uint8_t port);
void min_tx_start(uint8_t port);
void min_tx_byte(uint8_t port, uint8_t byte);

#endif
