/* saved_game_files_initialize @ 0x83726970 — set up save-game paths and mutexes */

#include <stdint.h>
#include <string.h>
#include "headers/saved_game_files_globals.h"
#include "headers/saved_game_files_path_type.h"
#include "headers/blam_data_globals.h"

#include "headers/mutex.h"
extern char *system_get_scratch_disk_root(void);
/* strncpy declared by <string.h> */
extern int _snprintf_0(char *, unsigned int, const char *, ...);
extern uint8_t create_mutex(mutex **mutex_reference);
extern void player_profiles_initialize(void);
extern void playlist_profiles_initialize(void);


void saved_game_files_initialize(void)
{
    const char *scratch_root;
    unsigned char initialized;

    memset(&saved_game_files_globals, 0, sizeof(saved_game_files_globals));
    scratch_root = system_get_scratch_disk_root();
    strncpy(saved_game_files_globals.paths[_saved_game_files_path_type_memory_unit_root], scratch_root, 0xFF);
    _snprintf_0(memory_unit_mapfile_name, 0xFF, "%s\\%s\\%s",
               saved_game_files_globals.paths[_saved_game_files_path_type_memory_unit_root], "saved", "hdmu.map");
    _snprintf_0(saved_game_files_globals.paths[_saved_game_files_path_type_savegame_root], 0xFF, "%s\\%s",
               saved_game_files_globals.paths[_saved_game_files_path_type_memory_unit_root], "savegames");
    saved_game_files_globals.memory_units_dirty = 1;
    saved_game_files_globals.general_mutex = 0;
    saved_game_files_globals.mapfile_mutex = 0;

    initialized = 1;
    if ( !create_mutex(&saved_game_files_globals.general_mutex)
      || !create_mutex(&saved_game_files_globals.mapfile_mutex) )
        initialized = 0;
    saved_game_files_globals.initialized = initialized;

    player_profiles_initialize();
    playlist_profiles_initialize();
}
