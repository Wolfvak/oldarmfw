#include <asm.h>
.align 2

@ void load_sys_ctx(cpuctx_t *ctx)
@ preserves supervisor state and
@ loads the new context in its place
ARM_FUNC load_sys_ctx
	@ preserve kernel state in SVC stack
	mrs r3, cpsr
	stmfd sp!, {r3-r11, lr}

	@ load task state
	mov lr, r0

	ldr r0, [lr, #16*4] @ load task CPSR
	msr spsr, r0

	ldmia lr, {r0-r14}^	@ load task GPR

	ldr lr, [lr, #15*4] @ load task PC
	movs pc, lr


@ void restore_svc_kernel(void)
@ loads the saved regs from stack
ARM_FUNC restore_svc_kernel
	ldmfd sp!, {r3-r11, lr}
	msr cpsr, r3
	bx lr


@ void sched_yield(void)
@ gives up CPU control
@ to the supervisor
ARM_FUNC sched_yield
	svc 0
	bx lr
