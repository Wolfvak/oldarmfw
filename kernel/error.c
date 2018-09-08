#include <common.h>
#include <arm.h>

#include <task/cpuctx.h>

static const char *source_strings[] = {
	[XRQ_UNDEFINED] = "Undefined Instruction",
	[XRQ_PREFETCH] = "Prefetch Abort",
	[XRQ_DATA] = "Data Abort",
	[XRQ_FIQ] = "FIQ"
};

extern uint32_t __text_s, __text_e;
extern uint32_t __data_s, __data_e, __bss_s, __bss_e;

static int
check_inst_ptr(uint32_t *addr)
{
	if ((uintptr_t)addr & 3)
		return 0;

	if (addr < &__text_s || addr >= &__text_e)
		return 0;

	return 1;
}

static int
check_data_ptr(uint32_t *addr)
{
	if (addr < &__text_s || addr >= &__text_e)
		return 0;

	if (addr < &__data_s || addr >= &__data_e)
		return 0;

	if (addr < &__bss_s || addr >= &__bss_e)
		return 0;

	return 1;
}

void
handle_fatal_error(int source, cpuctx_t *ctx)
{
	while(1);
}
