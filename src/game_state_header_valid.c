/* game_state_header_valid @0x83684CA8 — validate a loaded game-state header against the running game: build
 * number must match the expected version string, the scenario (map) name must match the loaded scenario, and
 * the allocation-size checksum, player count, and cache-file checksum must all agree. Returns 1 if valid.
 *
 * Deviation: the two string compares are the decompiler's inlined byte loops (equivalent to strcmp);
 * reproduced verbatim. The `fatal` argument is unused in this release path. */

#include <stdint.h>
#include "headers/game_state_header.h"
#include "headers/game_state_globals.h"
#include "headers/blam_data_globals.h"

extern char *tag_get_name(int16_t tag_index);
extern uint32_t cache_files_get_checksum(void);

uint8_t game_state_header_valid(game_state_header *header, uint8_t fatal)
{
    const char *expected_version = "01.00.01.0563";
    const char *build_number = header->build_number;
    int version_difference;
    do
    {
        int c = (unsigned char)*build_number;
        version_difference = c - (unsigned char)*expected_version;
        if ( c == 0 )
            break;
        ++build_number;
        ++expected_version;
    }
    while ( !version_difference );
    if ( version_difference )
        return 0;

    const char *scenario_name = tag_get_name(global_scenario_index);
    const char *map_name = header->map_name;
    int name_difference;
    do
    {
        int c = (unsigned char)*map_name;
        name_difference = c - (unsigned char)*scenario_name;
        if ( c == 0 )
            break;
        ++map_name;
        ++scenario_name;
    }
    while ( !name_difference );
    if ( name_difference )
        return 0;

    if ( header->allocation_size_checksum != game_state_globals.allocation_size_checksum )
        return 0;
    if ( header->player_count != player_spawn_count )
        return 0;
    if ( header->cache_file_checksum != cache_files_get_checksum() )
        return 0;
    return 1;
}
