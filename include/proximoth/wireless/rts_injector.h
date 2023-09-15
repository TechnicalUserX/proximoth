#ifndef PROXIMOTH_RTS_INJECTOR_H
#define PROXIMOTH_RTS_INJECTOR_H

#include <proximoth/proximoth.h>

#define PROXIMOTH_FRAME_CONTROL_RTS 0b10110100

extern pthread_t proximoth_rts_injector_thread;

extern atomic_bool proximoth_rts_injector_thread_shutdown_request;

extern uint64_t proximoth_rts_total_injected;

extern suseconds_t proximoth_rts_interval;

void* proximoth_rts_injector(void* arg);

void proximoth_rts_injector_wait(void);

#endif