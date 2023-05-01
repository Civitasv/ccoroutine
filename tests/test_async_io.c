#include "async_io.h"
#include <stdio.h>
#include <errno.h>

#define BUFSIZE 2048

void test()
{
	FILE *fp = fopen("cmake_install.cmake", "r");
	char buf[BUFSIZE];
	struct aiocb *aio = async_read(fp, buf, BUFSIZE);

	while (aio_error(aio) == EINPROGRESS)
		;

	int ret = aio_return(aio);

	fclose(fp);

	if (ret > 0) {
		printf("Got %d bytes:\n", ret);
	} else {
		printf("READ FAILED!!!\n");
	}
}

int main()
{
	test();
}