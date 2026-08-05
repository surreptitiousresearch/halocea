#pragma once
/* recorded_animation_definition — one scenario recorded-animation element (64 bytes, DB layout). */

#include <stdint.h>
#include "tag_data.h"

typedef struct recorded_animation_definition
{
    char             name[32];                  /* 0x00 */
    uint8_t  version;                   /* 0x20 — 1-based playback codec version */
    uint8_t  flags;                     /* 0x21 */
    uint8_t  unit_control_data_version; /* 0x22 */
    uint8_t  pad0;                      /* 0x23 */
    uint16_t ticks;                     /* 0x24 */
    uint16_t pad1;                      /* 0x26 */
    int              pad2[1];                   /* 0x28 */
    tag_data         animation_data;            /* 0x2C — the recorded event stream */
} recorded_animation_definition;                /* 64 bytes */
