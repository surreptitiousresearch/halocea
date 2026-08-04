#include <stdint.h>
#include "headers/widget_state.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/blam_data_globals.h"


/* DB symbol @0x837860C8 is _rasterizer_widget_set_texture (leading underscore kept); the
 * non-underscore name is the entry thunk @0x836A63B8 in rasterizer_widget_set_texture.c. */
int _rasterizer_widget_set_texture(int16_t stage_index, int bitmap_group_index, int16_t sequence_index)
{
    widget_state_new.stage_index = stage_index;

    if (bitmap_group_index == -1)
    {
        widget_state_new.bitmap_group_index = global_rasterizer_data->glow.index;
        widget_state_new.sequence_index = sequence_index;
    }
    else
    {
        widget_state_new.bitmap_group_index = bitmap_group_index;
        widget_state_new.sequence_index = sequence_index;
    }
    return 0;
}
