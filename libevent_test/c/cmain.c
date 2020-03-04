#include "../inc.h"

static const int PORT = 9995;
//const char* SERVER_IP = "127.0.0.1";
const char* SERVER_IP = "192.168.2.164";

#define SENDBUFSIZE 1024
char g_szSendBuf[SENDBUFSIZE];
#define RECVBUFSIZE 1024
char g_szRecvBuf[RECVBUFSIZE];

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

	if (reg_signal(base) != 0) return -1;

	evutil_socket_t connect_fd = -1;
	struct bufferevent* bev = bufferevent_socket_new(base, connect_fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		return -1;
	}

	bufferevent_setcb(bev, conn_readcb, NULL, conn_eventcb, NULL);
	bufferevent_enable(bev, EV_READ | EV_WRITE);

	struct sockaddr_in sai;
	memset(&sai, 0, sizeof(sai));
	sai.sin_family = AF_INET;
	sai.sin_port = htons(PORT);
	sai.sin_addr.s_addr=inet_addr(SERVER_IP);
	if(bufferevent_socket_connect(bev, (struct sockaddr*)&sai, sizeof(sai)) != 0){
		fprintf(stderr, "Connect failed!"); return -1;
	}

	event_base_dispatch(base);
	event_base_free(base);
	printf("done\n");
	return 0;
	FUNC_LEAVE;
}

static void conn_readcb(struct bufferevent *bev, void *user_data)
{
	FUNC_ENTER;
	struct evbuffer *input = bufferevent_get_input(bev);

	size_t len = evbuffer_get_length(input);
	if (len == 0) {
		printf("read len is 0\n");
		//bufferevent_free(bev);
	}
	bufferevent_read(bev, g_szRecvBuf, len);
	printf("read:%s\n", g_szRecvBuf);

	if (len > 0)
	{
		g_szRecvBuf[len] = '\0';
		int nTest = atoi(g_szRecvBuf);
		evutil_snprintf(g_szSendBuf, SENDBUFSIZE, "%d", nTest + 1);
		bufferevent_write(bev, g_szSendBuf, strlen(g_szSendBuf));
	}
	FUNC_LEAVE;
}

static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	FUNC_ENTER;
	if (events & BEV_EVENT_CONNECTED){
		printf("Connection established.\n");
	} else{
		if (events & BEV_EVENT_EOF) {
			printf("Connection closed.\n");
		}
		else if (events & BEV_EVENT_ERROR) {
#ifdef UNIQS_PLATFORM_WINDOWS
			printf("Got an error on the connection: %d\n", errno);
#else
			printf("Got an error on the connection: %s\n", strerror(errno));
#endif
		}

		/* None of the other events can happen here, since we haven't enabled
		 * timeouts */
		bufferevent_free(bev);
	}
	FUNC_LEAVE;
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	FUNC_ENTER;
	struct event_base* base = (struct event_base*)user_data;
	struct timeval delay = { 2, 0 };

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
	FUNC_LEAVE;
}

