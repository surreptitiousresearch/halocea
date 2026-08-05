/* hud_initialize_unit_interface_for_new_map @0x836D6668 — zero the unit-HUD globals then prime each local
 * player's HUD state so all vitality/flash trackers start "invalid" (-1).
 * Attested void return: 0/1 callers consume r3 (the decompiler threaded memset's return through). */

#include <stdint.h>
#include <string.h>
#include "headers/unit_hud_globals.h"

extern void initialize_hud_state(unit_hud_state *hud_state);
void hud_initialize_unit_interface_for_new_map(void)
{
    unit_hud_globals_definition *globals = unit_hud_globals;

    memset(unit_hud_globals, 0, sizeof(unit_hud_globals_definition));
    for ( int i = 0; i < 2; i = (int16_t)(i + 1) )
    {
        unit_hud_state *state = &globals->hud_states[i];
        /* DEVIATION: collapsed verbatim-inlined copy of initialize_hud_state@0x836D65E8 (zero-xref donor) into a call; single pointer param, no arg constant-folding needed. */
        initialize_hud_state(state);
        state->sound_flags = 0;
        memset(state->last_sound_handles, -1, sizeof(state->last_sound_handles));
    }
}
