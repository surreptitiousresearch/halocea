/* ai_initialize_for_new_map @0x836EA2E8 — bring up the whole AI subsystem for a new map: zero the globals, set
 * the active/control/grenade/dialogue flags and per-player chatter timers, then init the path, actor, prop,
 * encounter, script, and communication subsystems and clear the spatial-effects ring. */

#include "headers/ai_globals.h"
extern void *memset(void *dst, int value, unsigned int n);

extern void paths_initialize_for_new_map(void);
extern void actors_initialize_for_new_map(void);
extern void props_initialize_for_new_map(void);
extern void encounters_initialize_for_new_map(void);
extern void ai_script_initialize_for_new_map(void);
extern void ai_communication_initialize_for_new_map(void);

extern void *ai_flush_spatial_effects(void);
void ai_initialize_for_new_map(void)
{
    memset(ai_globals, 0, sizeof(ai_globals_t));
    ai_globals->ai_active = 1;
    ai_globals->ai_has_control_data = 1;
    ai_globals->first_encounterless_actor_index = -1;
    ai_globals->grenades_enabled = 1;
    ai_globals->dialogue_triggers_enabled = 1;
    ai_globals->last_chatter_time[0] = -1;
    ai_globals->last_chatter_time[1] = -1;
    ai_globals->last_talk_time[0] = -1;
    ai_globals->last_talk_time[1] = -1;
    ai_globals->last_shout_time[0] = -1;
    ai_globals->last_shout_time[1] = -1;

    paths_initialize_for_new_map();
    actors_initialize_for_new_map();
    props_initialize_for_new_map();
    encounters_initialize_for_new_map();
    ai_script_initialize_for_new_map();
    ai_communication_initialize_for_new_map();

    /* DEVIATION: collapsed inlined copy of ai_flush_spatial_effects@0x836E98C0 (zero-xref donor) back into a call; donor takes no args and its memset return value is unused here, matching the existing pattern where this host already ignores memset's return. */
    ai_flush_spatial_effects();
    ai_globals->ai_initialized_for_map = 1;
}
