#include "ccoroutine.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void context_switching()
{
	ucontext_t ctx = { 0 };

	getcontext(&ctx); // store context
	printf("Context switching example\n");
	sleep(1);
	setcontext(&ctx); // jump to location of ctx
}

void another(uint32_t *var, uint32_t val)
{
	*var = val; // 3. assignment
}

void control_flow()
{
	uint32_t var = 0;
	ucontext_t ctx = { 0 }, back = { 0 };

	getcontext(&ctx); // 1. store current context in ctx

	ctx.uc_stack.ss_sp = calloc(1, MINSIGSTKSZ);
	ctx.uc_stack.ss_size = MINSIGSTKSZ;
	ctx.uc_stack.ss_flags = 0;

	ctx.uc_link = &back;

	makecontext(&ctx, (void (*)())another, 2, &var, 100);

	// 2. store current context in back and move control to ctx
	swapcontext(&back, &ctx);

	printf("var = %d\n", var); // 4. print
}

void *hello_world(ccoroutine *coro, void *userdata)
{
	printf("%s\n", (char *)userdata);
	ccoroutine_yield(coro, (void *)1);
	printf("END: %s \n", (char *)userdata);
	return (void *)2;
}

void ccoroutine_test()
{
	ccoroutine *coro = ccoroutine_create(hello_world, "hello from 1");
	ccoroutine *coro2 = ccoroutine_create(hello_world, "hello from 2");

	while (!ccoroutine_finished(coro) || !ccoroutine_finished(coro2)) {
		ccoroutine_resume(coro);
		ccoroutine_resume(coro2);
	}

	ccoroutine_destroy(coro);
	ccoroutine_destroy(coro2);
}

int main()
{
	ccoroutine_test();
	return 0;
}