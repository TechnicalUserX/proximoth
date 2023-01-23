#ifndef PROXIMOTH_FRAME_SNIFFER_H
#define PROXIMOTH_FRAME_SNIFFER_H

#include "proximoth.h"

#define IEEE80211_FRAME_CONTROL_CTS 0b11000100


extern pthread_t proximoth_frame_sniffer_thread;

extern bool proximoth_frame_sniffer_thread_shutdown_request;

extern double proximoth_cts_catch_period;

extern uint64_t proximoth_cts_total_catched;

extern struct timeval proximoth_cts_last_time;

extern uint64_t proximoth_total_frames_catched;

extern int8_t proximoth_cts_last_signal_strength;

struct ieee80211_request_to_send{
    
    struct{ // Frame Control
        byte_t version:2;
        byte_t type:2;
        byte_t subtype:4;
        byte_t flags;
    };
    uint16_t duration;
    byte_t receiver_address[6];
    byte_t transmitter_address[6];
    
};

struct ieee80211_clear_to_send{
    
    struct{ // Frame Control
        byte_t version:2;
        byte_t type:2;
        byte_t subtype:4;
        byte_t flags;
    };
    uint16_t duration;
    byte_t receiver_address[6];
    
};

void* proximoth_frame_sniffer(void* arg);

#endif