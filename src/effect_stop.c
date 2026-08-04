/* effect_stop @0x836E2088 — stop a playing effect. Object-driven effects (flag 0x2) jump to the
 * definition's designated stop event (loop_stop_index) and mark themselves stopping (flag 0x4), or flag
 * themselves finished (flag 0x8) if there is no valid stop event; `and_delete` additionally marks the
 * effect for deletion once stopped (flag 0x20). Non-object effects are deleted immediately. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/effect_datum.h"
#include "headers/effect_definition.h"
#include "headers/effect_flags.h"
#include "headers/blam_data_globals.h"


extern void *datum_try_and_get(const data_array *data, int index);
extern void effect_set_event(int effect_index, int16_t event_index);
extern void effect_delete(int effect_index);

void effect_stop(int effect_index, uint8_t and_delete)
{
    effect_datum *effect = datum_try_and_get(effect_data, effect_index);
    __int16 flags;
    effect_definition *definition;
    int stop_event;

    if ( !effect )
        return;

    flags = effect->flags;
    definition = TAG_GET(effect_definition, effect->definition_index);

    if ( (flags & (1u << _effect_loop_bit)) != 0 )
    {
        if ( and_delete )
            effect->flags = flags | (1u << _effect_delete_on_stop_bit);
        else
            effect->flags = flags & ~(1u << _effect_delete_on_stop_bit);

        stop_event = definition->loop_stop_index;
        if ( stop_event < 0 || (stop_event + 1) >= definition->events.count )
        {
            effect->flags |= (1u << _effect_stopped_bit);
        }
        else
        {
            effect_set_event(effect_index, stop_event + 1);
            effect->flags |= (1u << _effect_stopping_bit);
        }
    }
    else
    {
        effect_delete(effect_index);
    }
}
