#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"


void action_uncover_modify_color(int actor_index, real_argb_color *color)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    const real_argb_color *source = actor->state.action_data.___u0.uncover.sneaking
        ? global_real_argb_blue
        : global_real_argb_lightblue;

    *color = *source;
}
