#ifndef PXIPROTO_H__

typedef struct {
	uint8_t id;
	uint8_t argc;

	uint8_t dev;
	uint8_t func;

	uint32_t state;
	uint32_t arg[0];
} pxi_hdr;

#endif /* PXIPROTO_H__ */
