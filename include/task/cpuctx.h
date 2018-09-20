#pragma once

#include <common.h>

typedef struct {
	uint32_t gpr[16];	/* r0-r15 */
	uint32_t cpsr;
} cpuctx_t;

static inline void
cpuctx_clear(cpuctx_t *ctx) {
	memset(ctx, 0, sizeof(*ctx));
}

static inline void
cpuctx_init(cpuctx_t *ctx, uint32_t sp, uint32_t pc, uint32_t psr) {
	ctx->gpr[13] = sp;
	ctx->gpr[14] = 0;
	ctx->gpr[15] = pc;
	ctx->cpsr = psr;
}
