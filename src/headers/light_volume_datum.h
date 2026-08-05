#pragma once
#include <stdint.h>
/* light_volume_datum — one live light-volume widget datum (8 bytes; DB-verified). */

typedef struct light_volume_datum
{
    int16_t          identifier;        /* 0x00 */
    uint16_t flags;             /* 0x02 */
    int              definition_index;  /* 0x04 */
} light_volume_datum;                   /* 8 bytes */
