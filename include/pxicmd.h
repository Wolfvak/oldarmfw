#ifndef PXICMD_H__
#define PXICMD_H__

#include <stdint.h>

typedef struct {
	uint8_t id;
	uint8_t argc;

	uint8_t dev;
	uint8_t func;

	uint16_t code;
	uint16_t state;

	uint32_t arg[0];
} pxicmd;

#define PXIHDR_BSIZE(argc)	(sizeof(pxicmd) + (4 * (argc)))
#define PXIHDR_ARG(h, n)	((h)->arg[(n)])

enum {
	PXICMD_STATE_NEW,
	PXICMD_STATE_DONE,
	PXICMD_STATE_ERROR,
	PXICMD_STATE_QUEUED,
	PXICMD_STATE_WORKING,
};

enum {
	PXICMD_CODE_OK,
	PXICMD_CODE_IOERR,
	PXICMD_CODE_ARGERR,
	PXICMD_CODE_DRVERR,
	PXICMD_CODE_TOOMANY,
};


#ifdef ARM9
void pxicmd_reply(pxicmd *cmd);
void pxicmd_init(void);
#endif /* ARM9 */

#endif /* PXICMD_H__ */
