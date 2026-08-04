#pragma once
/* Bound to DB enum types_enum_values _BE16AFEF5ABAF4A0EE16248C659FE15A (2026-07-20 BLOCKED-retry). */

/* weapon_trigger_definition.overcharged_action (__int16 @0x50). DB-AUTHORITATIVE (was a GUESS before
 * extraction). Canonical DB names below; the single consumer (weapon_update) keeps its guessed
 * identifiers as same-value aliases. Prior guess corrected: value 2 was "discharge" (DB _trigger_overcharged_fire). */
enum weapon_overcharged_action
{
    _trigger_overcharged_none    = 0,
    _weapon_overcharged_action_none = 0,       /* legacy consumer alias */
    _trigger_overcharged_explodes = 1,
    _weapon_overcharged_action_explode = 1,    /* legacy consumer alias */
    _trigger_overcharged_fire    = 2,
    _weapon_overcharged_action_discharge = 2,  /* legacy consumer alias (guessed name) */
    NUMBER_OF_TRIGGER_OVERCHARGED_ACTIONS = 3,
};
