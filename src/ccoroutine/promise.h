#ifndef CCOROUTINE_PROMISE_H_
#define CCOROUTINE_PROMISE_H_

#include <stdio.h>

struct Promise;

typedef void Resolve(struct Promise *promise, void *resolve_data);
typedef void Reject(struct Promise *promise, void *reject_data);
typedef void Callback(
		struct Promise *promise,
		Resolve *resolve,
		Reject *reject
);

enum State { PENDING = 0, FULFILLED, REJECTED };

struct Promise {
	// object method start
	Callback	*callback;
	Resolve		*resolve;
	Reject		*reject;
	// object method end

	enum State 	state;
	void 		*userdata;
	void 		*resolve_data;
	void 		*reject_data;
};

// static method
struct Promise *ccoroutine_make_promise(Callback *callback, void *userdata);

void 		ccoroutine_await(struct Promise *promise);

#endif // CCOROUTINE_PROMISE_H_