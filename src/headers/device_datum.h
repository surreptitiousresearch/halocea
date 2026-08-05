#pragma once
/* _device_datum + device_datum — the live device object payload (DB-verified). */

#include <stdint.h>
#include "_object_datum.h"

typedef struct _device_datum
{
    unsigned int flags;                /* 0x00 */
    int16_t      power_group_index;    /* 0x04 */
    unsigned char _pad0[2]; /* db-verified padding */
    float        power;                /* 0x08 */
    float        power_velocity;       /* 0x0C */
    int16_t      position_group_index; /* 0x10 */
    unsigned char _pad1[2]; /* db-verified padding */
    float        position;             /* 0x14 */
    float        position_velocity;    /* 0x18 */
    int16_t      delay_ticks;          /* 0x1C */
} _device_datum;                       /* 32 bytes */

typedef struct device_datum
{
    int           definition_index;    /* 0x000 */
    _object_datum object;              /* 0x004 */
    _device_datum device;              /* 0x1F4 (500) */
} device_datum;                        /* 532 bytes */
