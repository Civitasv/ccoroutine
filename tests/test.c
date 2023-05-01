#include "ccoroutine.h"
#include "promise.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void callback(struct Promise *promise, Resolve *resolve, Reject *reject)
{
	sleep(2);
	if (0) {
		promise->resolve(promise, promise->userdata);
	} else {
		promise->reject(promise, "ERROR");
	}
}

struct Promise *resolve_after_2_seconds(void *userdata)
{
	struct Promise *promise = ccoroutine_make_promise(callback, userdata);

	return promise;
}

void coroutine_fun(ccoroutine *coro, void *userdata)
{
	ccoroutine_yield(coro, resolve_after_2_seconds(userdata));
}

void ccoroutine_test()
{
	ccoroutine *coro = ccoroutine_create(coroutine_fun, "hello from 1");

	{
		struct Promise *promise =
			ccoroutine_resume(coro); // 这里会立即返回

		ccoroutine_await(promise);

		if (promise->state == FULFILLED)
			printf("RESOLVE: %s", (char *)promise->resolve_data);
		else
			printf("REJECT: %s", (char *)promise->reject_data);
	}

	ccoroutine_destroy(coro);
}

int main()
{
	ccoroutine_test();
	return 0;
}