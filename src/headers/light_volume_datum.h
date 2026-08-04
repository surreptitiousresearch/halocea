#pragma once
/* light_volume_datum — one live light-volume widget datum (8 bytes; DB-verified). */

typedef struct light_volume_datum
{
    __int16          identifier;        /* 0x00 */
    unsigned __int16 flags;             /* 0x02 */
    int              definition_index;  /* 0x04 */
} light_volume_datum;                   /* 8 bytes */
