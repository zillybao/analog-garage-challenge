#include "ringbuffer.h"

void ringbuf_init(ringbuf_t* rb, uint8_t* storage, size_t capacity){
    rb->storage = storage;
    rb->capacity = capacity;
    atomic_init(&(rb->head), 0);
    atomic_init(&(rb->tail), 0);
}

 /* producer only */
bool ringbuf_push(ringbuf_t* rb, uint8_t byte){
    /*
    false if full -> head is right behind tail
    leave space for 1
    */

    size_t head = atomic_load(&(rb->head));
    size_t tail = atomic_load(&(rb->tail));

    // if ((rb->head == rb->tail - 1) || (rb->head == rb->capacity - 1 && rb->tail == 0)) return false;
    if ((head == tail - 1) || (head == rb->capacity - 1 && tail == 0)) return false;

    *(rb->storage + head) = byte;

    // rb->head = (rb->head + 1) % rb->capacity;
    atomic_store(&(rb->head), (head + 1) % rb->capacity);

    return true;
}

 /* consumer only */
bool ringbuf_pop(ringbuf_t* rb, uint8_t* out){
    /*
    false if empty -> head == tail
    */
    size_t head = atomic_load(&(rb->head));
    size_t tail = atomic_load(&(rb->tail));
    if (head == tail) return false;
    *out = *(rb->storage + tail);
    // rb->tail = (rb->tail + 1) % rb->capacity;
    atomic_store(&(rb->tail), (tail + 1) % rb->capacity);
    return true;
}

size_t ringbuf_get_used(const ringbuf_t* rb){
    /*
    head - tail
    capacity - (tail - head)
    */
    size_t head = atomic_load(&(rb->head));
    size_t tail = atomic_load(&(rb->tail));

    return head >= tail ? head - tail : rb->capacity - (tail - head);
}

size_t ringbuf_get_free(const ringbuf_t* rb){
    return rb->capacity - ringbuf_get_used(rb) - 1;
}

