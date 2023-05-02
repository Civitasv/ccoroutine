#include "async_io.h"
#include <aio.h>
#include <asm-generic/errno.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

struct Data {
	struct aiocb *aio;
	FILE *fp;
};

#define BUFSIZE 2048
struct Data async_read_test()
{
	struct Data data;

	FILE *fp = fopen("cmake_install.cmake", "r");
	char buf[BUFSIZE];

	struct aiocb *aio = async_read(fp, buf, BUFSIZE);

	data.aio = aio;
	data.fp = fp;

	return data;
}

void sync_read_test()
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

void test()
{
	clock_t start_time, end_time;
	double total_time;

	start_time = clock();

	const int count = 1000;
#if 0
	for (int i = 0; i < count; i++)
		// 0.004207
		sync_read_test();
#else
	struct Data data[count];

	for (int i = 0; i < count; i++) {
		data[i] = async_read_test();
	}

	for (int i = 0; i < count; i++) {
		struct Data item = data[i];
		while (aio_error(item.aio) == EINPROGRESS)
			;

		int ret = aio_return(item.aio);
		printf("Read %d elements from file\n", ret);

		free(item.fp);
	}
#endif

	end_time = clock();

	total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	printf("Total time taken: %f seconds\n", total_time);
}

int main()
{
	test();
}