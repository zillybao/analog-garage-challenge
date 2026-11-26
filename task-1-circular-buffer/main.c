#include "ringbuffer.h"
#include <assert.h>
#include <stdio.h>

#define BUFFER_CAPACITY 10

int main(){
    // init
    ringbuf_t rb1;
    size_t capacity = BUFFER_CAPACITY;
    uint8_t buffer[capacity];
    ringbuf_init(&rb1, buffer, capacity);

    // push until full
    for (uint8_t i = 0; i < capacity - 1; i++){
        assert(ringbuf_push(&rb1, i) == true);
    }
    assert(ringbuf_push(&rb1, 0) == false);
    printf("push until full test pass\n");

    // pop until empty
    uint8_t val;
    for (uint8_t i = 0; i < capacity - 1; i++){
        assert(ringbuf_pop(&rb1, &val) == true);
        assert(val == i);
    }
    assert(ringbuf_pop(&rb1, &val) == false);
    printf("pop until empty test pass\n");

    // half full get_used / get_free, wraparound check
    ringbuf_push(&rb1, 10);
    ringbuf_push(&rb1, 20);
    ringbuf_push(&rb1, 30);
    ringbuf_push(&rb1, 40);

    assert(rb1.head == 3);
    assert(rb1.tail == 9);
    printf("wraparound test pass\n");

    assert(ringbuf_get_used(&rb1) == 4);
    assert(ringbuf_get_free(&rb1) == 5);
    printf("get used and get free test pass\n");

    // spsc stress

    return 0;
}

