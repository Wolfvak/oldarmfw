#include <asm.h>
#include <hw/mpu.h>
.align 2
.arm

#define SVC_STACK_SIZE	(512)
#define IRQ_STACK_SIZE	(1024)

ARM_FUNC reloc
	@ Disable any processor interrupts
	msr cpsr_c, #(SR_SVC | SR_NOINT)

	@ Invalidate IC, DC, drain write buffer
	mov r0, #0
	mcr p15, 0, r0, c7, c5, 0
	mcr p15, 0, r0, c7, c6, 0
	mcr p15, 0, r0, c7, c10, 4


	@ Disable MPU, caches, TCMs, etc
	ldr r0, =0x2078
	mcr p15, 0, r0, c1, c0, 0


	@ Setup TCMs
	ldr r1, =0x4000000A	@ DTCM @ 0x40000000 / 16KB (16KB)
	ldr r2, =0x00000024	@ ITCM @ 0x00000000 / 32KB (128MB)
	mcr p15, 0, r1, c9, c1, 0
	mcr p15, 0, r2, c9, c1, 1

	@ Enable TCMs
	orr r0, r0, #0x50000
	mcr p15, 0, r0, c1, c0, 0


	@ Setup MPU
	adr r1, _reloc_mpu_regs
	ldmia r1, {r1-r10}

    mcr p15, 0, r1, c6, c0, 0	@ MPU Regions 0-7
    mcr p15, 0, r2, c6, c1, 0
    mcr p15, 0, r3, c6, c2, 0
    mcr p15, 0, r4, c6, c3, 0
    mcr p15, 0, r5, c6, c4, 0
    mcr p15, 0, r6, c6, c5, 0
    mcr p15, 0, r7, c6, c6, 0
    mcr p15, 0, r8, c6, c7, 0

	mcr p15, 0, r9, c5, c0, 3	@ Instruction Access
	mcr p15, 0, r10, c5, c0, 2	@ Data Access

	mov r1, #0b10000010
	mov r2, #0b00000010
	mov r3, #0b00000010

	mcr p15, 0, r1, c2, c0, 1	@ Instruction Cacheing
	mcr p15, 0, r2, c2, c0, 0	@ Data Cacheing
	mcr p15, 0, r3, c3, c0, 0	@ Write Buffer Enable

    @ Enable MPU
    orr r0, r0, #1
    mcr p15, 0, r0, c1, c0, 0


	@ Relocate sections
	ldr r0, =__vector_lma
	ldr r1, =__vector_s
	ldr r2, =__vector_e
	bl _reloc_copy_sect

	ldr r0, =__text_lma
	ldr r1, =__text_s
	ldr r2, =__text_e
	bl _reloc_copy_sect

	ldr r0, =__data_lma
	ldr r1, =__data_s
	ldr r2, =__data_e
	bl _reloc_copy_sect

	ldr r0, =__bss_s
	ldr r1, =__bss_e
	mov r2, #0
	1:
		cmp r0, r1
		strne r2, [r0], #4
		bne 1b


	@ Setup stacks
	msr cpsr_c, #(SR_IRQ | SR_NOINT)
	ldr sp, =(_irq_stk + IRQ_STACK_SIZE)

	msr cpsr_c, #(SR_SVC | SR_NOINT)
	ldr sp, =(_svc_stk + SVC_STACK_SIZE)


	@ Branch to low level hardware initialization
	ldr pc, =ll_init


@ void _reloc_copy_sect(void *sect_lma, void *sect_addr, void *sect_end)
@ addresses must be aligned to 16 bytes
_reloc_copy_sect:
	cmp r1, r2
	bxeq lr	@ intentionally 'eq' to catch unaligned pointers

	ldmia r0!, {r3-r6}
	stmia r1!, {r3-r6}
	b _reloc_copy_sect


_reloc_mpu_regs:
    .word MPU_REGION(0x01FF8000, MPU_SZ_32K)	@ ITCM
    .word MPU_REGION(0x08000000, MPU_SZ_1M)		@ WRAM
    .word MPU_REGION(0x10000000, MPU_SZ_2M)		@ MMIO
    .word MPU_REGION(0x18000000, MPU_SZ_8M)		@ VRAM
    .word MPU_REGION(0x1FF00000, MPU_SZ_1M)		@ AXIRAM
    .word MPU_REGION(0x20000000, MPU_SZ_256M)	@ FCRAM
    .word MPU_REGION(0x40000000, MPU_SZ_16K)	@ DTCM
    .word MPU_REGION(0xFFFF0000, MPU_SZ_64K)	@ BootROM

    .word 0x50000055 @ Instruction Access Permissions
    .word 0x51111111 @ Data Access Permissions
.pool

BSS_DATA _irq_stk, IRQ_STACK_SIZE, 3
BSS_DATA _svc_stk, SVC_STACK_SIZE, 3
