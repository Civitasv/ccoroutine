#include <aio.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <ucontext.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include "ccoroutine.h"
#include "async_io.h"
#include "promise.h"

void callback(struct Promise *promise, Resolve *resolve, Reject *reject)
{
	sleep(2);
	if (0) {
		promise->resolve(promise, promise->userdata);
	} else {
		promise->reject(promise, "ERROR");
	}
}

struct Promise *async_io_example(void *userdata)
{
	struct Promise *promise = ccoroutine_make_promise(callback, userdata);

	return promise;
}

#define BUFSIZE 2048
void sync_read()
{
	FILE *fp = fopen("./test", "r");
	if (fp == NULL) {
		printf("FILE NOT EXISTS!!!\n");
	}

	char *buf = calloc(1, BUFSIZE);
	size_t read = fread(buf, sizeof(char), BUFSIZE, fp);
	printf("Read %zu elements from file\n", read);

	fclose(fp);
	free(buf);
}

void coroutine_fun(ccoroutine *coro, void *userdata)
{
	FILE *fp = fopen("./test", "r");
	if (fp == NULL) {
		printf("FILE NOT EXISTS!!!\n");
		ccoroutine_yield(coro, NULL);
	}

	char *buf = calloc(1, BUFSIZE);

	struct aiocb *aio = async_read(fp, buf, BUFSIZE);

	ccoroutine_yield(coro, aio);

	fclose(fp);
	free(buf);
}

void ccoroutine_test(int count)
{
	ccoroutine *coros[count];
	struct aiocb *aios[count];

	for (int i = 0; i < count; i++) {
		ccoroutine *coro =
			ccoroutine_create(coroutine_fun, "hello from 1");
		struct aiocb *aio = ccoroutine_resume(coro); // 这里会立即返回
		coros[i] = coro;
		aios[i] = aio;
	}

	for (int i = 0; i < count; i++) {
		while (aio_error(aios[i]) == EINPROGRESS)
			;

		int ret = aio_return(aios[i]);

		if (ret > 0) {
			printf("A Got %d bytes:\n", ret);
		} else {
			printf("A READ FAILED!!!\n");
		}

		ccoroutine_destroy(coros[i]);
	}
}

void file_read_sync(int count)
{
	for (int i = 0; i < count; i++)
		sync_read();
}

int main()
{
	clock_t start_time, end_time;
	double total_time;

	start_time = clock();

	// 0.455170
	// file_read_sync(100000);
	// 1.935576
	ccoroutine_test(100000);

	end_time = clock();
	total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	printf("Total time taken: %f seconds\n", total_time);
	return 0;
}