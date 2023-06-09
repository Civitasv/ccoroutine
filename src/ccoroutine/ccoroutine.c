#include "ccoroutine.h"
#include <stdlib.h>
#include <signal.h>

static void ccoroutine_entry_point(ccoroutine *coro, void *userdata)
{
	coro->function(coro, userdata);
	coro->is_finished = true;
	ccoroutine_yield(coro, NULL);
}

ccoroutine *ccoroutine_create(ccoroutine_function function, void *userdata)
{
	ccoroutine *coro = calloc(1, sizeof(*coro));

	coro->is_finished = false;
	coro->function = function;
	coro->resume_context.uc_stack.ss_sp = calloc(1, MINSIGSTKSZ);
	coro->resume_context.uc_stack.ss_size = MINSIGSTKSZ;
	coro->resume_context.uc_link = 0;

	getcontext(&coro->resume_context);
	makecontext(&coro->resume_context, (void (*)())ccoroutine_entry_point,
		    2, coro, userdata);
	return coro;
}

void *ccoroutine_resume(ccoroutine *coro)
{
	if (coro->is_finished)
		return NULL;
	swapcontext(&coro->suspend_context, &coro->resume_context);
	return coro->yield_value;
}

void ccoroutine_yield(ccoroutine *coro, void *value)
{
	coro->yield_value = value;
	swapcontext(&coro->resume_context, &coro->suspend_context);
}

void ccoroutine_destroy(ccoroutine *coro)
{
	free(coro->resume_context.uc_stack.ss_sp);
	free(coro);
}

bool ccoroutine_finished(ccoroutine *coro)
{
	return coro->is_finished;
}