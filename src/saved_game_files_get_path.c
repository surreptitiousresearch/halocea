/* saved_game_files_get_path @0x83726AD0 — return the save-file path string for the given path type (an index
 * into the globals' fixed path table). */

#include <stdint.h>
#include "headers/saved_game_files_globals.h"
#include "headers/blam_data_globals.h"


char *saved_game_files_get_path(int16_t path_type)
{
    return saved_game_files_globals.paths[path_type];
}
