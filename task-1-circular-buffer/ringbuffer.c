#include "ringbuffer.h"

void ringbuf_init(ringbuf_t* rb, uint8_t* storage, size_t capacity){
    rb->storage = storage;
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
}

 /* producer only */
bool ringbuf_push(ringbuf_t* rb, uint8_t byte){
    /*
    false if full -> head is right behind tail
    leave space for 1
    */
    if ((rb->head == rb->tail - 1) || (rb->head == rb->capacity - 1 && rb->tail == 0)) return false;
    *(rb->storage + rb->head) = byte;
    rb->head = (rb->head + 1) % rb->capacity;
    return true;
}

 /* consumer only */
bool ringbuf_pop(ringbuf_t* rb, uint8_t* out){
    /*
    false if empty -> head == tail
    */
    if (rb->head == rb->tail) return false;
    *out = *(rb->storage + rb->tail);
    rb->tail = (rb->tail + 1) % rb->capacity;
    return true;
}

size_t ringbuf_get_used(const ringbuf_t* rb){
    /*
    head - tail
    capacity - (tail - head)
    */
    if (rb->head >= rb->tail) return rb->head - rb->tail;
    return rb->capacity - (rb->tail - rb->head);
}

size_t ringbuf_get_free(const ringbuf_t* rb){
    return rb->capacity - ringbuf_get_used(rb) - 1;
}

