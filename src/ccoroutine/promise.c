#include "promise.h"
#include <stdlib.h>

static void _resolve(struct Promise *promise, void *resolve_data)
{
	promise->state = FULFILLED;
	promise->resolve_data = resolve_data;
}

static void _reject(struct Promise *promise, void *reject_data)
{
	promise->state = REJECTED;
	promise->reject_data = reject_data;
}

struct Promise *ccoroutine_make_promise(Callback *callback, void *userdata)
{
	struct Promise *promise = calloc(1, sizeof(*promise));
	promise->state = PENDING;
	promise->callback = callback;

	promise->resolve = _resolve;
	promise->reject = _reject;

	promise->userdata = userdata;
	promise->resolve_data = NULL;
	promise->reject_data = NULL;

	return promise;
}

void ccoroutine_await(struct Promise *promise)
{
	promise->callback(promise, promise->resolve, promise->reject);

	while (promise->state == PENDING)
		;
}