#pragma once

/* actor_variant_definition — AI variant tag definition (568 bytes, DB-verified header).
 * Full member set per authoritative reference (C:\noav\headers_ref\) and DB
 * (types_members). The former opaque tail is now the real items/unit/change_colors block. */

#include "tag_reference.h"
#include "tag_block.h"
#include "actor_movement_switching_properties.h"
#include "actor_ranged_combat_properties.h"
#include "actor_grenade_combat_properties.h"
#include "actor_item_properties.h"
#include "actor_unit_properties.h"

typedef struct actor_variant_definition
{
    unsigned int    flags;                                       /* 0x00 */
    tag_reference   actor_reference;                             /* 0x04 */
    tag_reference   unit_reference;                              /* 0x14 */
    tag_reference   major_upgrade_reference;                     /* 0x24 */
    unsigned int    unused[6];                                   /* 0x34 */
    actor_movement_switching_properties movement_switching;      /* 0x4C */
    actor_ranged_combat_properties      ranged_combat;           /* 0x64 */
    actor_grenade_combat_properties     grenade_combat;          /* 0x180 (384) */
    actor_item_properties               items;                   /* 0x1C0 (448) */
    actor_unit_properties               unit;                    /* 0x200 (512) */
    unsigned int    unused2[3];                                  /* 0x220 (544) */
    tag_block       change_colors;                               /* 0x22C (556) */
} actor_variant_definition;                                      /* 0x238 = 568 bytes */
