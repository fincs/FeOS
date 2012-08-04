#include "common.h"

typedef struct
{
	int sock;
	struct sockaddr_in* addr;
} stream_data;

static int     cb_open (void* data);
static void    cb_close(void* data);
static ssize_t cb_write(void* data, const char* buf, size_t len);
static ssize_t cb_read (void* data, char* buf,       size_t len);

static stream_t stream_st =
{
	cb_open,
	cb_close,
	cb_write,
	cb_read,
	NULL
};

FILE* fopentcp(struct sockaddr_in* addr)
{
	if (!addr) return NULL;
	stream_data* d = (stream_data*) malloc(sizeof(stream_data));
	d->addr = addr;
	if (!d) return NULL;
	FILE* f = FeOS_OpenStream(&stream_st, d);
	if (!f)
	{
		free(d);
		return NULL;
	}
	return f;
}

int cb_open(void* _d)
{
	stream_data* d = (stream_data*) _d;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return errno;

	if (connect(sock, (struct sockaddr*) d->addr, sizeof(struct sockaddr_in)) < 0)
	{
		int rc = errno;
		shutdown(sock, 0);
		closesocket(sock);
		return rc;
	}

	d->sock = sock;
	return 0;
}

void cb_close(void* _d)
{
	stream_data* d = (stream_data*) _d;
	int sock = d->sock;

	shutdown(sock, 0);
	closesocket(sock);
	free(d);
}

ssize_t cb_write(void* d, const char* buf, size_t len)
{
	return send(((stream_data*)d)->sock, buf, len, 0);
}

ssize_t cb_read(void* d, char* buf, size_t len)
{
	return recv(((stream_data*)d)->sock, buf, len, 0);
}
