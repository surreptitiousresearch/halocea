/* playlist_profile_get @0x83782688 — reads a saved playlist profile by index into variant; index -1
 * means "no saved profile", so the playlist cursor is simply advanced and the call reports failure.
 *
 * DEVIATION: the decompiler rendered the no-profile branch as game_engine_playlist_next(-1, variant), a
 * bogus 2-argument call. Disasm confirms game_engine_playlist_next takes no arguments here — r3/r4 are
 * simply left holding this function's own incoming parameters (unread by the callee) at the call site. */

#include <stdint.h>
#include "headers/game_variant.h"

extern void game_engine_playlist_next(void);
extern int playlist_profile_read(int index, game_variant *variant);

uint8_t playlist_profile_get(int playlist_profile_index, game_variant *variant)
{
    if (playlist_profile_index == -1)
    {
        game_engine_playlist_next();
        return 0;
    }

    return playlist_profile_read(playlist_profile_index, variant);
}
