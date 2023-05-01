#ifndef CCOROUTINE_AIO_H_
#define CCOROUTINE_AIO_H_

#include <aio.h>
#include <stdio.h>

#define CCOROUTINE_ASYNC_CHOICE 0 // 0 for aio, 1 for io_uring

struct aiocb *async_read(FILE *fp, char *buf, int bytes);

#endif // CCOROUTINE_AIO_H_