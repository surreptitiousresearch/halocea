#pragma once
/* Bound to DB enum types_enum_values _94C965335B3A1B885C440F133C6B6771 (2026-07-20 BLOCKED-retry). */

/* _weapon_definition.secondary_trigger_mode (__int16 @0x24). DB-AUTHORITATIVE (was a GUESS before
 * extraction) — the compiled enum matches the prior reconstruction exactly. Canonical DB names below;
 * the consumer (weapon_update) keeps its identifiers as same-value aliases. */
enum weapon_secondary_trigger_mode
{
    _weapon_secondary_trigger_normal                             = 0,
    _weapon_secondary_trigger_mode_normal                       = 0, /* legacy consumer alias */
    _weapon_secondary_trigger_slaved_to_primary                 = 1,
    _weapon_secondary_trigger_mode_slaved_to_primary            = 1, /* legacy consumer alias; proven by weapon_update */
    _weapon_secondary_trigger_inhibits_primary                  = 2,
    _weapon_secondary_trigger_mode_inhibits_primary             = 2, /* legacy consumer alias; proven by weapon_update */
    _weapon_secondary_trigger_loads_alternate_ammunition        = 3,
    _weapon_secondary_trigger_mode_loads_alternate_ammunition   = 3, /* legacy consumer alias */
    _weapon_secondary_trigger_loads_multiple_primary_ammunition = 4,
    _weapon_secondary_trigger_mode_loads_multiple_primary_ammunition = 4, /* legacy consumer alias */
    NUMBER_OF_WEAPON_SECONDARY_TRIGGER_MODES = 5,
};
