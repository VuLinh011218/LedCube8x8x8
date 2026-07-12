#include "ring_buffer.h"

void rb_init(ring_buffer_t *g_rb)
{
    g_rb->head = g_rb->tail = 0;
}

bool rb_push(ring_buffer_t *g_rb, uint8_t data)
{
    uint16_t next = (g_rb->head + 1) % RB_SIZE;
    if (next == g_rb->tail)
    {
        return false;
    }
    g_rb->buf[g_rb->head] = data;
    g_rb->head            = next;
    return true;
}

bool rb_pop(ring_buffer_t *g_rb, uint8_t *data)
{
    if (g_rb->head == g_rb->tail)
    {
        return false;
    }
    *data      = g_rb->buf[g_rb->tail];
    g_rb->tail = (g_rb->tail + 1) % RB_SIZE;
    return true;
}
