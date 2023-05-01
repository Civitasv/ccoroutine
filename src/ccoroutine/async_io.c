#include "async_io.h"
#include <aio.h>
#include <stdio.h>
#include <stdlib.h>

struct aiocb *async_read(FILE *fp, char *buf, int bytes)
{
	struct aiocb *aio = calloc(1, sizeof(*aio));
	if (aio == NULL) {
		return NULL;
	}

	aio->aio_buf = buf;
	aio->aio_fildes = fileno(fp);
	aio->aio_nbytes = bytes;
	aio->aio_offset = 0;

	int result = aio_read(aio);
	if (result < 0) {
		free(aio);
		return NULL;
	}

	return aio;
}