#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

#define va_start __builtin_va_start
#define va_list __builtin_va_list
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end

void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memset(void *s, int c, size_t n);
char *strcpy(char *dest, const char *src);

int printf(const char *fmt, ...);
int sprintf(char *str, const char *fmt, ...);

static inline void
breakup(void) {
	__builtin_trap();
}

#define ASZ(x)	(sizeof(x) / sizeof(*(x)))
#define BIT(x)	(1 << (x))
