#pragma once
/* weapon_magazine_definition — one element of a weapon definition's magazines tag_block.
 * Layout DB-verified against type 'weapon_magazine_definition' (112 bytes, 2026-07-14). */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"
#include "weapon_magazine_interface_definition.h"

typedef struct weapon_magazine_definition
{
    unsigned int  flags;                       /* 0x00 */
    int16_t       rounds_recharged_per_second; /* 0x04 */
    int16_t       rounds_total_initial;        /* 0x06 */
    int16_t       rounds_total_maximum;        /* 0x08 */
    int16_t       rounds_loaded_maximum;       /* 0x0A */
    int16_t       rounds_unused[4];            /* 0x0C */
    float         reload_time;                 /* 0x14 */
    int16_t       rounds_reloaded;             /* 0x18 */
    uint16_t pad;                      /* 0x1A */
    float         chamber_time;                /* 0x1C */
    weapon_magazine_interface_definition interface_definition; /* 0x20 */
    tag_reference reloading_effect;            /* 0x38 */
    tag_reference chambering_effect;           /* 0x48 */
    int           unused[3];                   /* 0x58 */
    tag_block     ammunition_objects;          /* 0x64 */
} weapon_magazine_definition;                  /* 112 bytes */
