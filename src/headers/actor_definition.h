#pragma once
/* actor_definition — AI actor tag definition ('actr', 1272 bytes). Per an authoritative structure
 * dump (C:\halo\headers_ref\actor_definition.h, 2026-07-09), the FULL top-level layout is now known:
 * perception/moving/looking/unopposable/panic/defensive/pursuit/berserk/firing_position/communication
 * are all named structs. Only `unused_ranged` (268 bytes, an unused/legacy ranged-weapon properties
 * copy) and a 192-byte tail past `communication` (unused[32] + unused_major_upgrade_reference +
 * unused2[12], none of it read by any reconstructed function) remain opaque.
 * unused3[2] holds the variant-definition tag reference index used by the migration code. */

#include "actor_moving_properties.h"
#include "actor_defensive_properties.h"
#include "actor_perception_properties.h"
#include "actor_looking_properties.h"
#include "actor_unopposable_properties.h"
#include "actor_panic_properties.h"
#include "actor_pursuit_properties.h"
#include "actor_berserk_properties.h"
#include "actor_firing_position_properties.h"
#include "actor_communication_properties.h"
#include "tag_reference.h"

typedef struct actor_definition
{
    unsigned int    flags;          /* 0x00 */
    unsigned int    flags2;         /* 0x04 */
    unsigned int    unused3[3];     /* 0x08 — unused3[2] (0x10) = variant definition tag index */
    __int16         type;           /* 0x14 */
    unsigned __int16 pad;           /* 0x16 */
    actor_perception_properties  perception;      /* 0x18 (24) */
    actor_moving_properties      moving;           /* 0x80 (128) */
    actor_looking_properties     looking;           /* 0xA4 (164) */
    tag_reference   unused_ranged_weapon_reference;  /* 0x13C (316) */
    unsigned int    unused_ranged[67];                /* 0x14C (332) — legacy/unused ranged-weapon properties (DB field name; unread data) */
    tag_reference   unused_grenade_reference;          /* 0x258 (600) */
    actor_unopposable_properties  unopposable;          /* 0x268 (616) */
    actor_panic_properties        panic;                 /* 0x298 (664) */
    actor_defensive_properties    defensive;              /* 0x2D0 (720) */
    actor_pursuit_properties      pursuit;                 /* 0x33C (828) */
    actor_berserk_properties      berserk;                  /* 0x378 (888) */
    actor_firing_position_properties firing_position;         /* 0x3B8 (952) */
    actor_communication_properties   communication;             /* 0x3F8 (1016) */
    unsigned int    unused[32];                                   /* 0x438 (1080) — DB field name; unread data */
    tag_reference   unused_major_upgrade_reference;               /* 0x4B8 (1208) */
    unsigned int    unused2[12];                                  /* 0x4C8 (1224) — DB field name; unread data */
} actor_definition;                 /* 1272 bytes */
