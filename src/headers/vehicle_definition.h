#pragma once
/* vehicle_definition — vehicle tag definition ('vehi', 1008 bytes). Complete: the database models this
 * as three composed blocks (DB types/types_members, size 1008), each of which is now fully named in its
 * own header — object (_object_definition, 0x00, 380 bytes), unit (_unit_definition, 0x17C, 372 bytes),
 * and vehicle (_vehicle_definition, 0x2F0, 256 bytes). The vehicle block resolves the doc's "vehicle
 * tag's pathfinding radius (+908)", "ai_charge_repeat_time (+0x390)", "ai_strafing_stop_range
 * (+0x394)", and the movement-layer's oversteer/steering-angle citations (float indices [230]/[231]/
 * [232] from an earlier pass, i.e. bytes 920/924/928) all in one shot — plus `ai_destination_radius`
 * at byte 900, which `actor_destination_tolerance.c` reads (confirmed via source: it indexes the
 * ridden vehicle's tag as `float[225]`) for the doc's "unit tag +900" destination-tolerance citation —
 * that citation was a misnomer for the *vehicle* tag, not a generic unit tag. */

#include "_object_definition.h"
#include "_unit_definition.h"
#include "_vehicle_definition.h"

typedef struct vehicle_definition
{
    _object_definition  object;   /* 0x000 (0)   — shared object_definition payload */
    _unit_definition    unit;     /* 0x17C (380) — shared unit_definition payload */
    _vehicle_definition vehicle;  /* 0x2F0 (752) — vehicle-level payload */
} vehicle_definition; /* 0x3F0 = 1008 bytes */
