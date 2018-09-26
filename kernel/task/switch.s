#include <asm.h>
.align 2


@ void load_usr_ctx(cpuctx_t *ctx)
@ preserves supervisor state and
@ loads the new context in its place
ARM_FUNC load_usr_ctx
	@ preserve kernel state in SVC stack
	mrs r12, cpsr
	stmfd sp!, {r4-r12, lr}

	@ load task state
	mov lr, r0

	ldr r0, [lr, #16*4]	@ load task PSR
	msr spsr, r0

	ldmia lr, {r0-r14}^	@ load task GPR
	ldr lr, [lr, #15*4]	@ load task PC
	movs pc, lr			@ branch to task


@ all SVC instructions come from user/system mode
@ SVC handler saves registers R4-PC
@ restores kernel state
ARM_FUNC _exception_svc
	mrs r0, spsr
	ands r1, r0, #0xF
	cmpne r1, #0xF
	bne _svc_notusrsys		@ make sure the SVC was called from USR or SYS mode

	ldr r1, =sched_active
	ldr r1, [r1]
	str lr, [r1, #15*4]		@ preserve task PC
	str r0, [r1, #16*4]		@ preserve task PSR

	add r1, r1, #4*4
	stmia r1, {r4-r14}^		@ preserve necessary GPRs

	ldmfd sp!, {r4-r12, lr}	@ restore kernel state from stack
	msr cpsr, r12			@ should've been saved in load_usr_ctx
	bx lr

_svc_notusrsys:
	udf #0

.pool
