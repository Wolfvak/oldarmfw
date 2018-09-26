#include <asm.h>

.arm
.align 2

@ void *memset(void *s, int c, size_t n)
ARM_FUNC memset
	mov r3, r0
1:
	subs r2, r2, #1
	strneb r1, [r3], #1
	beq 1b
	bx lr
