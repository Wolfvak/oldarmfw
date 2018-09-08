#include <asm.h>

.macro TRAP_ENTRY xrq_id, mode
	msr cpsr_f, #(\xrq_id << 30)	@ preserve exception source (idea grabbed from fb3ds)
	msr cpsr_c, #(\mode | SR_NOINT)	@ disable interrupts
.endm


ARM_FUNC _exception_undef
	TRAP_ENTRY XRQ_UNDEFINED, SR_UND
	b _exception_fatal

ARM_FUNC _exception_prefetch
	TRAP_ENTRY XRQ_PREFETCH, SR_ABT
	b _exception_fatal

ARM_FUNC _exception_data
	TRAP_ENTRY XRQ_DATA, SR_ABT
	b _exception_fatal

ARM_FUNC _exception_fiq
	TRAP_ENTRY XRQ_FIQ, SR_FIQ


ARM_FUNC _exception_fatal
	ldr sp, =(_fatal_stk - 18*4)
	stmia sp, {r0-r7}				@ store the non-banked GPRs

	mrs r1, cpsr
	lsr r0, r1, #30					@ r0 = fatal exception source

	mrs r2, spsr
	str r2, [sp, #16*4]				@ store exception PSR

	ands r2, r2, #0xF
	orreq r2, r2, #0xF				@ if the abort happened in USR
									@ switch to SYS instead

	orr r2, r2, #(0x10 | SR_NOINT)	@ with interrupts disabled
	add r3, sp, #8*4
	msr cpsr_c, r2
	stmia r3, {r8-r14}				@ store remaining prev mode registers
									@ including banked FIQ if needed
	msr cpsr_c, r1

	str lr, [sp, #15*4]				@ store exception PC
	mov r1, sp
	bl handle_fatal_error

	mov r0, #0
	1:
		mcr p15, 0, r0, c7, c0, 4
		b 1b


@ all SVC instructions come from user/system mode
@ save registers R4-LR in active task
@ and retore kernel state afterwards
ARM_FUNC _exception_svc
	msr cpsr_c, #(SR_SVC | SR_NOINT) @ forcefully disable interrupts

	sub sp, sp, #14*4
	stmia sp, {r4-r14}^	@ preserve GPRs
						@ R12 doesn't really need to be saved

	str lr, [sp, #11*4]	@ preserve task PC

	mrs r0, spsr
	str r0, [sp, #12*4] @ preserve task PSR

	@ memcpy(&sched_active_cpuctx()->gpr[4], saved_regs, 13*4)
	bl sched_active_cpuctx
	add r0, r0, #4*4
	ldmia sp!, {r2-r7}
	stmia r0!, {r2-r7}
	ldmia sp!, {r2-r8}
	stmia r0!, {r2-r8}
	add sp, sp, #4
	b restore_svc_kernel


.section .bss.fatal_stack
.align 3

.space (256 * 4)
_fatal_stk:
.global _fatal_stk
