#ifndef PROXIMOTH_CTS_SNIFFER_H
#define PROXIMOTH_CTS_SNIFFER_H

#include <proximoth/proximoth.h>

#define PROXIMOTH_CTS_FRAME  0xC4 // 0b11000100

extern pthread_t proximoth_cts_sniffer_thread;

extern atomic_bool proximoth_cts_sniffer_thread_shutdown_request;

extern double proximoth_cts_catch_period;

extern uint64_t proximoth_cts_total_catched;

extern struct timeval proximoth_cts_last_time;

extern uint64_t proximoth_total_frames_catched;

extern int8_t proximoth_cts_last_signal_strength;

struct proximoth_request_to_send {
    
    struct{ // Frame Control
        proximoth_byte_t version:2;
        proximoth_byte_t type:2;
        proximoth_byte_t subtype:4;
        proximoth_byte_t flags;
    };
    uint16_t duration;
    proximoth_byte_t receiver_address[6];
    proximoth_byte_t transmitter_address[6];
    
};

struct proximoth_clear_to_send {
    
    struct{ // Frame Control
        proximoth_byte_t version:2;
        proximoth_byte_t type:2;
        proximoth_byte_t subtype:4;
        proximoth_byte_t flags;
    };
    uint16_t duration;
    proximoth_byte_t receiver_address[6];
    
};

void* proximoth_cts_sniffer(void* arg);

#endif