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

void coroutine_fun(ccoroutine *coro, void *userdata)
{
	ccoroutine_yield(coro, (void *)1);
	ccoroutine_yield(coro, (void *)2);
}

void ccoroutine_test()
{
	ccoroutine *coro = ccoroutine_create(coroutine_fun, "hello from 1");
	assert(ccoroutine_resume(coro) == (void *)1);
	assert(ccoroutine_resume(coro) == (void *)2);
	assert(ccoroutine_resume(coro) == NULL);

	ccoroutine_destroy(coro);
}

int main()
{
	ccoroutine_test();

	return 0;
}