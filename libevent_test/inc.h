#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>// atoi
#include <signal.h>

#include "../common_header/platform.h"

#ifndef UNIQS_PLATFORM_WINDOWS
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif // !WIN32

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "../common_header/debugfunc.h"
#include "regsig.h"

