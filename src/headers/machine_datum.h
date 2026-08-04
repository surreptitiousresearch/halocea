#pragma once
/* _machine_datum + machine_datum — the live machine (device) object payload (DB-verified). */

#include "device_datum.h"
#include "real_point3d.h"

typedef struct _machine_datum
{
    unsigned int flags;             /* 0x00 — bit 3: open on damage */
    int          door_open_ticks;   /* 0x04 */
    real_point3d elevator_position; /* 0x08 */
} _machine_datum;                   /* 20 bytes */

typedef struct machine_datum
{
    int            definition_index; /* 0x000 */
    _object_datum  object;           /* 0x004 */
    _device_datum  device;           /* 0x1F4 (500) */
    _machine_datum machine;          /* 0x214 (532) */
} machine_datum;                     /* 552 bytes */
