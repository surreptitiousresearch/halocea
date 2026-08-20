/* scenery_update @0x837E9E38 — per-tick scenery update: if the object's "animated" flag (+500 bit 0x1) isn't
 * set, nothing to do. Otherwise runs its animation one tick (kind = affects_game_state); when that reports
 * _animation_will_restart_on_next_frame it decrements object.animation.state.frame_index (+0xD2 == 210).
 * Always returns 1.
 *
 * DEVIATION: the guard was reconstructed with its sense inverted (`result != ...`). The binary steps the
 * frame index only on the equal path: `cmpwi cr6,r11,2` @0x837E9E98 / `bne cr6,loc_837E9EC4` @0x837E9E9C
 * branches *past* `lhz r11,0xD2(r31)` @0x837E9EA0 and `sth r10,0xD2(r31)` @0x837E9EA8. (+0xD2 is the frame
 * index, not a loop counter; +0xD4 is the interpolation cursor.)
 *
 * Object header lookup matches the established salted object-header idiom (see vehicle_reset.c). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/scenery_datum.h"
#include "headers/animation_update_kind.h"
#include "headers/animation_state.h"
#include "headers/scenery_flags.h"
#include "headers/animation_update_result.h"
#include "headers/blam_data_globals.h"


#include "headers/animation_state.h"
extern int16_t animation_update_internal(animation_update_kind render_or_affects_game_state, int animation_graph_index, animation_state *state, int *triggered_sound_index);

uint8_t scenery_update(int scenery_index)
{
    scenery_datum *scenery = (scenery_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, scenery_index)->datum;

    if ( (scenery->scenery.flags & (1u << _scenery_self_animated_bit)) == 0 )
        return 1;

    int result = animation_update_internal(
        animation_update_kind_affects_game_state,
        scenery->object.animation.animation_graph_index,
        &scenery->object.animation.state,
        0);

    if ( result == _animation_will_restart_on_next_frame )
        --scenery->object.animation.state.frame_index;

    return 1;
}
