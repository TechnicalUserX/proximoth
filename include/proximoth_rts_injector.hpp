#ifndef _PROXIMOTH_RTS_INJECTOR_HPP
#define _PROXIMOTH_RTS_INJECTOR_HPP

#include "proximoth.hpp"

#define IEEE80211_FRAME_CONTROL_RTS 0b10110100

extern pthread_t proximoth_rts_injector_thread;

extern bool proximoth_rts_injector_thread_shutdown_request;

extern uint64_t proximoth_rts_total_injected;

extern uint32_t proximoth_rts_interval;

void* proximoth_rts_injector(void* arg);

void proximoth_rts_injector_wait(void);

#endif