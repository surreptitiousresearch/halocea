#pragma once
#include <stdint.h>
/* decal_wrap_parameters — per-decal-type wrapping tuning (DB types_members-confirmed, 16 bytes).
 * The global table decal_wrap_parameters[] is indexed by decal_definition::type. */

/* Bare struct TAG (not a typedef): the runtime global array shares this name, so the object
 * 'decal_wrap_parameters' and the type 'struct decal_wrap_parameters' must coexist (shadow-global
 * pattern). Consumers only index the array, so no 'struct' keyword is needed at use sites. */
struct decal_wrap_parameters
{
    float           minimum_wrap_angle;          /* 0x00 — degrees; surfaces beyond this are "deviant" */
    float           minimum_skip_angle;          /* 0x04 */
    float           radius_exclusion_multiplier; /* 0x08 */
    uint8_t wrap;                         /* 0x0C — non-zero: wrap the decal around deviant surfaces */
};                                                /* 16 bytes */

extern struct decal_wrap_parameters decal_wrap_parameters[];
