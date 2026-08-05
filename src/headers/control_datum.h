#pragma once
/* _control_datum + control_datum — the live control (device) object payload (DB-verified). */

#include <stdint.h>
#include "device_datum.h"

typedef struct _control_datum
{
    unsigned int flags;              /* 0x00 */
    int16_t      hud_override_index; /* 0x04 */
    int16_t      pad;                /* 0x06 */
} _control_datum;                    /* 8 bytes */

typedef struct control_datum
{
    int            definition_index; /* 0x000 */
    _object_datum  object;           /* 0x004 */
    _device_datum  device;           /* 0x1F4 (500) */
    _control_datum control;          /* 0x214 (532) */
} control_datum;                     /* 540 bytes */
