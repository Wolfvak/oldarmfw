#pragma once

#define SR_USR	(0x10)
#define SR_FIQ	(0x11)
#define SR_IRQ	(0x12)
#define SR_SVC	(0x13)
#define SR_ABT	(0x17)
#define SR_UND	(0x1B)
#define SR_SYS	(0x1F)

#define SR_PMODE_MASK	(0xF)

#define SR_THUMB	(1 << 5)
#define SR_NOFIQ	(1 << 6)
#define SR_NOIRQ	(1 << 7)
#define SR_NOINT	(SR_NOFIQ | SR_NOIRQ)

#define XRQ_UNDEFINED	(0)
#define XRQ_PREFETCH	(1)
#define XRQ_DATA		(2)
#define XRQ_FIQ			(3)

#ifdef __ASSEMBLER__
.macro ARM_FUNC f
	.arm
	.align 2
	.section .text.\f, "ax", %progbits
	.global \f
	.type \f, %function
\f:
.endm

.macro THUMB_FUNC f
	.thumb
	.align 1
	.section .text.\f, "ax", %progbits
	.global \f
	.type \f, %function
\f:
.endm

.macro BSS_DATA d, l, a=0
	.section .bss.\d
	.align \a
	.global \d
	\d:
	.space (\l)
.endm
#endif
