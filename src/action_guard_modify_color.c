/* action_guard_modify_color @0x838246FC — pick a debug color for an actor's "guard" state: violet/pink/
 * purple by flag combination (action_data[8]/[9]/[10]) when the primary flag is set, salmon otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"


void action_guard_modify_color(uint16_t actor_index, real_argb_color *color)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    const real_argb_color *selected;

    if ( actor->state.action_data.___u0.guard.cower )
    {
        if ( actor->state.action_data.___u0.guard.cower_from_retreat )
            selected = global_real_argb_violet;
        else if ( actor->state.action_data.___u0.guard.cower_panicked )
            selected = global_real_argb_pink;
        else
            selected = global_real_argb_purple;
    }
    else
    {
        selected = global_real_argb_salmon;
    }

    *color = *selected;
}
