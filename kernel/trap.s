#include <asm.h>

#define FATAL_STACK_SIZE	(512)

.macro TRAP_ENTRY xrq_id, mode
	msr cpsr_f, #(\xrq_id << 30)	@ preserve exception source (idea grabbed from fb3ds)
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
	ldr sp, =(_fatal_stk + FATAL_STACK_SIZE - 18*4)
	stmia sp, {r0-r7}				@ store the non-banked GPRs

	mrs r1, cpsr
	orr r0, r1, #(SR_NOINT)
	msr cpsr_c, r0					@ disable interrupts

	lsr r0, r1, #30					@ r0 = fatal exception source

	mrs r2, spsr
	str r2, [sp, #16*4]				@ store exception PSR

	ands r2, r2, #0xF
	orreq r2, r2, #0xF				@ if the abort happened in USR
									@ switch to SYS instead

	orr r2, r2, #(0x10 | SR_NOINT)	@ with interrupts disabled
	add r3, sp, #8*4
	msr cpsr_c, r2
	nop
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


BSS_DATA _fatal_stk, FATAL_STACK_SIZE, 3
