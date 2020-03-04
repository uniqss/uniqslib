#pragma once

#include <event2/event.h>

int reg_signal(struct event_base* base);

static void signal_cb(evutil_socket_t, short, void *);

