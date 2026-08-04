/* player_profiles_initialize @ 0x83765990 — reset profile globals, build default profile */

#include <stdint.h>
#include "headers/player_profile.h"
extern void *memset(void *, int, unsigned int);

#include "headers/player_profile_globals.h"
/* DEVIATION: the decompiler put initialized@1988; the DB-verified layout has initialized@0xF80 (memset
 * size 0xF84) — disasm `stb _player_profile_globals.initialized`. Reusing the canonical header corrects it. */

extern void player_profile_build_default_profile(player_profile *profile, int player_index, uint8_t try_inherit);
/* extern corrected to match def (player_profile_create_default_profiles_on_disk.c): returns file_reference*. */
extern void player_profile_create_default_profiles_on_disk(void);

void player_profiles_initialize(void)
{
    memset(&player_profile_globals, 0, sizeof(player_profile_globals));
    /* recovered: &player_profile_globals -> &player_profile_globals.default_profile */
    player_profile_build_default_profile(&player_profile_globals.default_profile, 0, 0);
    player_profile_globals.initialized = 1;
    player_profile_create_default_profiles_on_disk(); /* attested void: r3-thread + cast dropped */
}
