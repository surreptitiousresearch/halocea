#pragma once
/* _control_definition + control_definition — the 'ctrl' tag body (DB-verified). */

#include <stdint.h>
#include "_object_definition.h"
#include "_device_definition.h"
#include "tag_reference.h"

typedef struct _control_definition
{
    int16_t       type;        /* 0x00 — 0 toggle, 1 on, 2 off, 3 call */
    int16_t       trigger;     /* 0x02 */
    float         call_value;  /* 0x04 */
    unsigned int  unused[20];  /* 0x08 */
    tag_reference on_effect;   /* 0x58 */
    tag_reference off_effect;  /* 0x68 */
    tag_reference deny_effect; /* 0x78 */
} _control_definition;         /* 136 bytes */

typedef struct control_definition
{
    _object_definition  object;  /* 0x000 (380 bytes) */
    _device_definition  device;  /* 0x17C (276 bytes) */
    _control_definition control; /* 0x290 (136 bytes) */
} control_definition;            /* 792 bytes */
