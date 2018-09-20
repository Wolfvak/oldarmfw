#include <asm.h>
.align 2
.arm

@ ARM9-specific exception vectors
.section .vector, "ax"
_vectors_start:
	ldr pc, _irq_vector
	_irq_vector: .word _exception_irq
	ldr pc, _fiq_vector
	_fiq_vector: .word _exception_fiq
	ldr pc, _svc_vector
	_svc_vector: .word _exception_svc
	ldr pc, _undef_vector
	_undef_vector: .word _exception_undef
	ldr pc, _prefetch_vector
	_prefetch_vector: .word _exception_prefetch
	ldr pc, _data_vector
	_data_vector: .word _exception_data
