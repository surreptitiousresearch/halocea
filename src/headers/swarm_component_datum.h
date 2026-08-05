#ifndef SWARM_COMPONENT_DATUM_H
#define SWARM_COMPONENT_DATUM_H

#include <stdint.h>
#include "real_point3d.h"
#include "swarm_wander_control.h"
#include "obey_individual_simple_control.h"

/* DB anonymous union _7739F9BA5DC5463F1692F968C57E1278 (36 bytes) — per-action control arm
 * ($-name kept verbatim; $ in identifiers is an MSVC extension). */
typedef union _7739F9BA5DC5463F1692F968C57E1278
{
    swarm_wander_control           wander; /* 0x00 */
    obey_individual_simple_control obey;   /* 0x00 */
} _7739F9BA5DC5463F1692F968C57E1278;

typedef struct swarm_component_datum
{
    int16_t         identifier;                  /* 0x00 */
    int16_t         flags;                       /* 0x02 */
    real_point3d    position;                    /* 0x04 */
    int             surface_index;               /* 0x10 */
    int             combat_target_prop_index;    /* 0x14 */
    uint8_t attached_to_unit_ticks;      /* 0x18 */
    uint8_t ground_ticks;                /* 0x19 */
    uint8_t attack_delay_ticks;          /* 0x1A */
    uint8_t pad;                         /* 0x1B */
    union _7739F9BA5DC5463F1692F968C57E1278 ___u9; /* 0x1C — DB anonymous union, defined above */
} swarm_component_datum;                         /* 0x40 */

#endif /* SWARM_COMPONENT_DATUM_H */
