#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#pragma once
#include <stdint.h>
#include <stdbool.h>

#define RB_SIZE 512

typedef struct
{
    uint8_t           buf[RB_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} ring_buffer_t;

void rb_init(ring_buffer_t *g_rb);
bool rb_push(ring_buffer_t *g_rb, uint8_t data);
bool rb_pop(ring_buffer_t *g_rb, uint8_t *data);
#endif
