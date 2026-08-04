#pragma once
/* scenario_unit_datum — unit-specific scenario placement data embedded in scenario_biped_datum /
 * scenario_vehicle_datum (16 bytes, DB types_members-confirmed layout). */

typedef struct scenario_unit_datum
{
    float        body_vitality; /* 0x0 */
    unsigned int flags;         /* 0x4 */
    unsigned int unused[2];     /* 0x8 */
} scenario_unit_datum; /* 16 bytes */
