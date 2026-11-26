#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>

typedef struct {
    uint8_t* storage;
    size_t capacity;
    _Atomic size_t head;
    _Atomic size_t tail;
} ringbuf_t;

void ringbuf_init(ringbuf_t* rb, uint8_t* storage, size_t capacity);
bool ringbuf_push(ringbuf_t* rb, uint8_t byte); /* producer only */
bool ringbuf_pop(ringbuf_t* rb, uint8_t* out); /* consumer only */
size_t ringbuf_get_used(const ringbuf_t* rb);
size_t ringbuf_get_free(const ringbuf_t* rb);

#endif
