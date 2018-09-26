#pragma once

#include <common.h>

typedef struct {
	uint32_t gpr[16];	/* r0-r15 */
	uint32_t cpsr;
} cpuctx_t;

static inline void
cpuctx_clear(cpuctx_t *ctx) {
	for (int i = 0; i < 16; i++)
		ctx->gpr[i] = 0;
	ctx->cpsr = 0;
}

static inline void
cpuctx_init(cpuctx_t *ctx, void *arg, void *sp, void *pc, uint32_t psr) {
	ctx->gpr[0] = (uint32_t)arg;
	ctx->gpr[13] = (uint32_t)sp;
	ctx->gpr[14] = 0;
	ctx->gpr[15] = (uint32_t)pc;
	ctx->cpsr = psr;
}
