#pragma once
/* weapon_ammunition_object — one element of weapon_magazine_definition.ammunition_objects:
   an equipment item that refills this magazine (28 bytes; layout from the database). */

#include <stdint.h>
#include "tag_reference.h"

typedef struct weapon_ammunition_object
{
    int16_t          rounds;    /* 0x00 — rounds granted per pickup */
    uint16_t unused[5]; /* 0x02 */
    tag_reference    object;    /* 0x0C — the equipment tag */
} weapon_ammunition_object;     /* 28 bytes */
