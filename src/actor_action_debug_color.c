/* actor_action_debug_color @0x837F3E30 — debug color for an actor's current action: starts black, and if
 * the actor's action class (word +54, per actor_action_class.c) has a dispatch table entry, takes that
 * entry's base color and lets its modify_color callback (if any) further adjust it in place.
 *
 * DEVIATION: the decompiler calls modify_color with zero arguments; the DB's action_specification struct
 * (action_specification.h) declares it `(actor_index, real_argb_color *color)`, and disasm confirms both
 * r3 (still actor_index) and r4 (&global_temporary_render_color) are live at the call site. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/action_specification.h"
#include "headers/real_argb_color.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


const real_argb_color * actor_action_debug_color(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    global_temporary_render_color = *global_real_argb_black;

    unsigned int action_class = actor->state.action;
    if ( action_class <= actor_action_avoid )
    {
        global_temporary_render_color = **global_action_functions[action_class].color;
        void (__fastcall *modify_color)(int, real_argb_color *) = global_action_functions[action_class].modify_color;
        if ( modify_color )
            modify_color(actor_index, &global_temporary_render_color);
    }

    return &global_temporary_render_color;
}
