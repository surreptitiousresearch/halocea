#pragma once
/* recorded_animation_definition — one scenario recorded-animation element (64 bytes, DB layout). */

#include "tag_data.h"

typedef struct recorded_animation_definition
{
    char             name[32];                  /* 0x00 */
    unsigned __int8  version;                   /* 0x20 — 1-based playback codec version */
    unsigned __int8  flags;                     /* 0x21 */
    unsigned __int8  unit_control_data_version; /* 0x22 */
    unsigned __int8  pad0;                      /* 0x23 */
    unsigned __int16 ticks;                     /* 0x24 */
    unsigned __int16 pad1;                      /* 0x26 */
    int              pad2[1];                   /* 0x28 */
    tag_data         animation_data;            /* 0x2C — the recorded event stream */
} recorded_animation_definition;                /* 64 bytes */
