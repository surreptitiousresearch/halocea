/* saved_game_files_dispose @ 0x83726A60 — free mutexes and profile state */
#include "headers/saved_game_files_globals.h"
extern void dispose_mutex(mutex *mutex_reference);
extern void player_profiles_dispose(void);
extern void playlist_profiles_dispose(void);
void saved_game_files_dispose(void)
{
    if ( saved_game_files_globals.general_mutex )
    {
        dispose_mutex(saved_game_files_globals.general_mutex);
        saved_game_files_globals.general_mutex = 0;
    }
    if ( saved_game_files_globals.mapfile_mutex )
    {
        dispose_mutex(saved_game_files_globals.mapfile_mutex);
        saved_game_files_globals.mapfile_mutex = 0;
    }
    player_profiles_dispose();
    /* decompiler artifact removed: playlist_profiles_dispose(player_profiles_dispose())
     * was r3-threading; both callees are zero-arg (defs), r3 at blr is callee residue. */
    playlist_profiles_dispose();
    saved_game_files_globals.initialized = 0;
}
