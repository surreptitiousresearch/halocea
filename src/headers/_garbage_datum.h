#pragma once
#include <stdint.h>
/* _garbage_datum — the garbage-level runtime fields, embedded at offset 0x22C (556) inside the composite
 * garbage_datum object datum: a despawn countdown timer. Layout from the database (24 bytes). */

typedef struct _garbage_datum
{
    int16_t destroy_timer;  /* 0x00 */
    int16_t pad;            /* 0x02 */
    int     unused[5];      /* 0x04 */
} _garbage_datum;           /* 24 bytes */
