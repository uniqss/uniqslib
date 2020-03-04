#include "regsig.h"
#include "../common_header/debugfunc.h"
#include <signal.h>

struct event *signal_event;

int reg_signal(struct event_base* base)
{
	signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);
	if (!signal_event || event_add(signal_event, NULL) < 0) {
		fprintf(stderr, "Could not create/add a signal event!\n");
		return -1;
	}

	return 0;
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	FUNC_ENTER;
	struct event_base* base = (struct event_base*)user_data;
	struct timeval delay = { 1, 0 };

	printf("Caught an interrupt signal; exiting cleanly in 1 seconds.\n");

	event_free(signal_event);
	event_base_loopexit(base, &delay);
	FUNC_LEAVE;
}

