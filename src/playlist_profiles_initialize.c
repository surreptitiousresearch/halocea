/* playlist_profiles_initialize @ 0x83781E98 — reset playlist-profile globals */

extern void *memset(void *, int, unsigned int);

#include "headers/playlist_profile_globals.h"
/* DEVIATION: the decompiler put initialized@0 (memset size 1); the DB-verified layout has initialized@0xA2
 * (memset size 0xA4) — disasm `stb _playlist_profile_globals.initialized`. Reusing the canonical corrects it. */

/* attested void: r3 at blr is memset residue; 0/1 callers consume (decompiler r3-threading killed) */
void playlist_profiles_initialize(void)
{
    memset(&playlist_profile_globals, 0, sizeof(playlist_profile_globals));
    playlist_profile_globals.initialized = 1;
}
