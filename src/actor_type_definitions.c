/* actor_type_definitions — the 16-slot actor-type dispatch table, recovered from the binary's .data
 * segment (pointer table @0x841852E0, records @0x84186558/0x84186758..0x84186898, name strings in
 * .rdata). Previously believed unrecoverable because actor_types_initialize decompiles to an empty
 * 16-iteration loop — the resolution: the table is STATIC DATA, not runtime-registered; the loop merely
 * invokes each type's `initialize` hook, and every one is NULL, so the optimizer emptied it.
 *
 * Slot sharing (16 slots, 12 records): slot 5 (stealth elite) aliases slot 0's elite record; slot 6
 * aliases slot 7 (marine); slot 12 (monitor) aliases slot 13's sentinel record; slot 14 ("none") aliases
 * slot 2's grunt record. Aliased slots share one record, not just one decide_action.
 *
 * race values as shipped: covenant infantry 4, human 2, flood combat 8, carrier 16, infection 32,
 * sentinel 64, mounted weapon 0. when_to_* values are the pursuit-desire tiers consumed by
 * actor_action_handle_lost_contact via actor_type_get_when_to_*. */

#include <stdint.h>
#include "headers/actor_type_definition.h"
#include "headers/actor_race.h"

#include "headers/real_vector3d.h"
extern void elite_decide_action(int actor_index);           /* 0x838299C8 */
extern void jackal_decide_action(int actor_index);          /* 0x838296F0 */
extern void grunt_decide_action(int actor_index);           /* 0x838293F8 */
extern void hunter_decide_action(int actor_index);          /* 0x83829260 */
extern void engineer_decide_action(int actor_index);        /* 0x83829030 */
extern void marine_decide_action(int actor_index);          /* 0x83828DC0 */
extern void crew_decide_action(int actor_index);            /* 0x83828B90 */
extern void flood_decide_action(int actor_index);           /* 0x8381F038 */
extern void infection_decide_action(int actor_index);       /* 0x83827468 */
extern void carrier_decide_action(int actor_index);         /* 0x838272D8 */
extern void sentinel_decide_action(int actor_index);        /* 0x83827100 */
extern void mounted_weapon_decide_action(int actor_index);  /* 0x83826FF8 */
extern void infection_swarm_control(int actor_index);       /* 0x83827710 */
extern void infection_swarm_aim_jump(int actor_index, int unit_index, float jump_magnitude, real_vector3d *jump_velocity); /* 0x83828848 */

/*                                                          name              race wsat wtp wsp pc swarm  init  decide_action                   swarm_control/aim_jump */
static actor_type_definition actor_type_elite          = { "elite",             _race_covenant, 0,   1,  1,  1, 0,   0, elite_decide_action,          0, 0 };                       /* @0x84186898 */
static actor_type_definition actor_type_jackal         = { "jackal",            _race_covenant, 2,   0,  2,  0, 0,   0, jackal_decide_action,         0, 0 };                       /* @0x84186878 */
static actor_type_definition actor_type_grunt          = { "grunt",             _race_covenant, 0,   0,  0,  0, 0,   0, grunt_decide_action,          0, 0 };                       /* @0x84186858 */
static actor_type_definition actor_type_hunter         = { "hunter",            _race_covenant, 0,   1,  1,  1, 0,   0, hunter_decide_action,         0, 0 };                       /* @0x84186838 */
static actor_type_definition actor_type_engineer       = { "engineer",          _race_covenant, 0,   0,  0,  0, 0,   0, engineer_decide_action,       0, 0 };                       /* @0x84186818 */
static actor_type_definition actor_type_marine         = { "marine",            _race_human, 0,   0,  0,  0, 0,   0, marine_decide_action,         0, 0 };                       /* @0x841867F8 */
static actor_type_definition actor_type_crew           = { "crew",              _race_human, 0,   0,  0,  0, 0,   0, crew_decide_action,           0, 0 };                       /* @0x841867D8 */
static actor_type_definition actor_type_flood          = { "flood",             _race_floodcombat, 0,   0,  0,  1, 0,   0, flood_decide_action,          0, 0 };                       /* @0x84186558 */
static actor_type_definition actor_type_infection      = { "infection",        _race_floodinfection, 2,   2,  2,  0, 1,   0, infection_decide_action, infection_swarm_control, infection_swarm_aim_jump }; /* @0x841867B8 */
static actor_type_definition actor_type_carrier        = { "flood carrier",    _race_floodcarrier, 0,   2,  2,  1, 0,   0, carrier_decide_action,        0, 0 };                       /* @0x84186798 */
static actor_type_definition actor_type_sentinel       = { "sentinel",         _race_sentinel, 0,   0,  0,  0, 0,   0, sentinel_decide_action,       0, 0 };                       /* @0x84186778 */
static actor_type_definition actor_type_mounted_weapon = { "mounted_weapon",    _race_none, 2,   2,  2,  0, 0,   0, mounted_weapon_decide_action, 0, 0 };                       /* @0x84186758 */

actor_type_definition *actor_type_definitions[16] =        /* @0x841852E0 */
{
    &actor_type_elite,          /*  0 elite */
    &actor_type_jackal,         /*  1 jackal */
    &actor_type_grunt,          /*  2 grunt */
    &actor_type_hunter,         /*  3 hunter */
    &actor_type_engineer,       /*  4 engineer */
    &actor_type_elite,          /*  5 stealth elite — shares the elite record */
    &actor_type_marine,         /*  6 (marine alias) */
    &actor_type_marine,         /*  7 marine */
    &actor_type_crew,           /*  8 crew */
    &actor_type_flood,          /*  9 flood combat form */
    &actor_type_infection,      /* 10 infection form (swarm) */
    &actor_type_carrier,        /* 11 flood carrier form */
    &actor_type_sentinel,       /* 12 monitor — shares the sentinel record */
    &actor_type_sentinel,       /* 13 sentinel */
    &actor_type_grunt,          /* 14 "none" — defaults to the grunt record */
    &actor_type_mounted_weapon, /* 15 mounted weapon */
};
