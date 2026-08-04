#include "headers/game_globals_definition.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/tag_reference.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_definition.h"
#include "headers/blam_data_globals.h"


int get_sound_length_in_ticks(int index)
{
    game_globals_multiplayer_information *info = (game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address;

    if ( info && index < info->sounds.count )
    {
        tag_reference *entries = (tag_reference *)info->sounds.address;
        if ( entries )
        {
            int tag_index = entries[index].index;
            if ( tag_index != -1 )
                return 30 * TAG_GET(sound_definition, tag_index)->runtime_maximum_play_time / 1000;   /* sound+132 */
        }
    }
    return 0;
}
