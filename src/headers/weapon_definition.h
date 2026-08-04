#pragma once
/* weapon_definition — weapon tag definition ('weap', 1288 bytes), per an authoritative structure dump
 * (C:\halo\headers_ref\weapon_definition.h / _weapon_definition.h, 2026-07-09). Composite layout
 * DB-verified against type 'weapon_definition' (2026-07-13): object (0x00, 380 bytes,
 * _object_definition) and item (0x17C, 396 bytes, _item_definition) shared base levels, then the
 * weapon-level payload (0x308, 512 bytes, _weapon_definition — split out to headers/_weapon_definition.h
 * on 2026-07-13). */

#include "_object_definition.h"
#include "_item_definition.h"
#include "_weapon_definition.h"

typedef struct weapon_definition
{
    _object_definition  object;               /* 0x00 — shared object payload (380 bytes) */
    _item_definition    item;                 /* 0x17C — shared item payload (396 bytes) */
    _weapon_definition  weapon;               /* 0x308 (776) — weapon-level payload (512 bytes) */
} weapon_definition; /* 0x508 = 1288 bytes */
