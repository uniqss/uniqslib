#include "inc.h"

int g_nTest = 0;

#define SENDBUFSIZE 1024
char g_szSendBuf[SENDBUFSIZE];
#define RECVBUFSIZE 1024
char g_szRecvBuf[RECVBUFSIZE];

const int PORT = 9995;

static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
static void conn_readcb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);

int main(int argc, char **argv)
{
	FUNC_ENTER;

#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	struct event_base* base = event_base_new();
	if (!base) {
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	struct evconnlistener* listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
			LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
			(struct sockaddr*)&sin, sizeof(sin));

	if (!listener) {
		fprintf(stderr, "Could not create a listener!Maybe the port is already used.\n");
		return 1;
	}

	if (reg_signal(base) != 0) return -1;

	event_base_dispatch(base);

	evconnlistener_free(listener);
	event_base_free(base);

	printf("done\n");
	return 0;
	FUNC_LEAVE;
}

static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
		struct sockaddr *sa, int socklen, void *user_data)
{
	FUNC_ENTER;
	struct event_base* base = (struct event_base*)user_data;

	struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}
	bufferevent_setcb(bev, conn_readcb, NULL, conn_eventcb, NULL);
	bufferevent_enable(bev, EV_READ | EV_WRITE);

	evutil_snprintf(g_szSendBuf, SENDBUFSIZE, "%d", g_nTest);
	bufferevent_write(bev, g_szSendBuf, strlen(g_szSendBuf));
	FUNC_LEAVE;
}

static void conn_readcb(struct bufferevent *bev, void *user_data)
{
	FUNC_ENTER;
	struct evbuffer *input = bufferevent_get_input(bev);

	evutil_socket_t fd = bufferevent_getfd(bev);

	size_t len = evbuffer_get_length(input);
	if (len == 0) {
		printf("read len is 0\n");
		// low water-flow not set,so can't free it here.
		//bufferevent_free(bev);
	}
	bufferevent_read(bev, g_szRecvBuf, len);
	printf("read:%s\n", g_szRecvBuf);

	if (len > 0)
	{
		g_szRecvBuf[len] = '\0';
		int nTest = atoi(g_szRecvBuf);
		if (nTest >= 4){
			nTest = 0;
		} else {
			evutil_snprintf(g_szSendBuf, SENDBUFSIZE, "%d", nTest + 1);
			bufferevent_write(bev, g_szSendBuf, strlen(g_szSendBuf));
		}
	}
	FUNC_LEAVE;
}

static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	FUNC_ENTER;
	if (events & BEV_EVENT_EOF) {
		printf("Connection closed.\n");
	} else if (events & BEV_EVENT_ERROR) {
#ifdef UNIQS_PLATFORM_WINDOWS
		printf("Got an error on the connection: %d\n", errno);
#else
		printf("Got an error on the connection: %s\n",strerror(errno));
#endif
	}
	/* None of the other events can happen here, since we haven't enabled
	 * timeouts */
	bufferevent_free(bev);
	FUNC_LEAVE;
}

