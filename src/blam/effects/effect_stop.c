/* effect_stop @0x836E2088 — stop a playing effect. The one branch is _effect_loop_bit (bit 1, mask 0x2;
 * rlwinm r8,r11,0,30,30 @0x836E20C8): a LOOPING effect jumps to the definition's designated stop event
 * (loop_stop_index + 1) and marks itself _effect_stopping_bit (0x4 @0x836E2138), or marks itself
 * _effect_stopped_bit (0x8 @0x836E214C) when there is no valid stop event; `and_delete` sets or clears
 * _effect_delete_on_stop_bit (0x20 @0x836E20F0 / 0x836E2100) first. A non-looping effect is deleted
 * immediately (bl effect_delete @0x836E2160). Nothing here consults the effect's owning object. */

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
    int16_t flags;
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
