#pragma once
/* _garbage_datum — the garbage-level runtime fields, embedded at offset 0x22C (556) inside the composite
 * garbage_datum object datum: a despawn countdown timer. Layout from the database (24 bytes). */

typedef struct _garbage_datum
{
    __int16 destroy_timer;  /* 0x00 */
    __int16 pad;            /* 0x02 */
    int     unused[5];      /* 0x04 */
} _garbage_datum;           /* 24 bytes */
