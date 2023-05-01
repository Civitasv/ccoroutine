#ifndef CCOROUTINE_H_
#define CCOROUTINE_H_

#include <ucontext.h>
#include <ucontext.h>
#include <stdbool.h>

typedef struct ccoroutine ccoroutine;
typedef void *(*ccoroutine_function)(ccoroutine *coro, void *userdata);

// coroutine handler
struct ccoroutine {
	ccoroutine_function 	function;		// 该协程要执行的函数
	ucontext_t 		suspend_context;	// 挂起的上下文
	ucontext_t 		resume_context;		// 当前执行的上下文
	void 			*yield_value;		// 返回值
	bool			is_coro_finished;	// 协程生命周期
};

// apis
ccoroutine	*ccoroutine_create(ccoroutine_function function, void* userdata);
void		*ccoroutine_resume(ccoroutine *coro);
void 		ccoroutine_yield(ccoroutine* coro, void *value);
void		ccoroutine_destroy(ccoroutine* coro);

#endif // CCOROUTINE_H_