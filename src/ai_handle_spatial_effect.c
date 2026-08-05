/* ai_handle_spatial_effect @0x836E98F0 — register a spatial AI stimulus (a noise/impact event at a world
 * point) so nearby actors can react to it. Maintains a 32-entry ring buffer of ai_spatial_effect events in
 * ai_globals, coalescing repeats of the same effect type at (nearly) the same location:
 *   - Scans the live ring [first_index, last_index). Entries older than 120 ticks are expired (type = -1)
 *     and their slots reclaimed (the head advances; an interior expired slot is remembered for reuse).
 *   - If a live entry of the same type sits within 1 world unit of the new point, the event merges into it:
 *     its count increments, and its position is either blended toward the new point (weight 1/count, when the
 *     entry was touched within the last 30 ticks) or overwritten (when it is older than that).
 *   - Otherwise a new entry is allocated — into a reclaimed interior slot if one was found this scan, else by
 *     pushing the ring tail (evicting the head when the ring is full).
 * Actors are notified (actors_handle_spatial_effect) only for a genuinely new or refreshed event, i.e. unless
 * the merge landed on an entry touched within the last 30 ticks (rate-limiting rapid repeats).
 *
 * DEVIATION: the position-blend weight is 1.0f / (float)count. The decompiler rendered the count->float
 * conversion as a __int64 pun (LODWORD = count, HIDWORD = current_tick - 30); the upper word is
 * register-packing noise — disasm shows std/lfd/fcfid of the sign-extended count only (0x836E9B7C-0x836E9BA0). */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/real_point3d.h"

extern int game_time_get(void);
extern float distance_squared3d(const real_point3d *a, const real_point3d *b);
extern void actors_handle_spatial_effect(int object_index, int16_t effect_type, const real_point3d *position, int16_t volume, int16_t count);

void ai_handle_spatial_effect(int object_index, const real_point3d *position, int16_t effect_type, int16_t volume, int16_t count)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    int current_tick = game_time_get();
    if ( volume <= 0 )
        return;

    ai_spatial_effect *effect = nullptr;
    uint8_t should_notify = 1;
    uint16_t reusable_slot_index = 0xFFFF;

    for ( int16_t i = ai_globals->spatial_effects_first_index;
            i != ai_globals->spatial_effects_last_index;
            i = (i + 1) & 0x1F )
    {
        uint8_t position_matches = 0;
        if ( effect_type == ai_globals->spatial_effects[i].type )
            position_matches =
                    distance_squared3d(&ai_globals->spatial_effects[i].position, position) < 1.0;

        if ( ai_globals->spatial_effects[i].last_tick > current_tick - 120 )
        {
            if ( position_matches )
            {
                effect = &ai_globals->spatial_effects[i];
                ++effect->count;
                should_notify = effect->last_tick < current_tick - 30;
                if ( effect->last_tick >= current_tick - 30 )
                {
                    float new_weight = (float)1.0 / (float)effect->count;
                    float old_weight = (float)1.0 - new_weight;
                    effect->position.n[0] = (old_weight * effect->position.n[0])
                            + (new_weight * position->n[0]);
                    effect->position.n[1] = (old_weight * effect->position.n[1])
                            + (new_weight * position->n[1]);
                    effect->position.n[2] = (old_weight * effect->position.n[2])
                            + (new_weight * position->n[2]);
                }
                else
                {
                    effect->position.n[0] = position->n[0];
                    effect->position.n[1] = position->n[1];
                    effect->position.n[2] = position->n[2];
                }
                break;
            }
        }
        else
        {
            ai_globals->spatial_effects[i].type = -1;
            if ( i == ai_globals->spatial_effects_first_index )
                ai_globals->spatial_effects_first_index = (i + 1) & 0x1F;
            else
                reusable_slot_index = i;
        }
    }

    if ( !effect )
    {
        int16_t new_slot_index;
        if ( reusable_slot_index == 0xFFFF )
        {
            new_slot_index = ai_globals->spatial_effects_last_index;
            ai_globals->spatial_effects_last_index = (new_slot_index + 1) & 0x1F;
            if ( ai_globals->spatial_effects_last_index == ai_globals->spatial_effects_first_index )
                ai_globals->spatial_effects_first_index =
                        (ai_globals->spatial_effects_first_index + 1) & 0x1F;
        }
        else
        {
            new_slot_index = reusable_slot_index;
        }
        effect = &ai_globals->spatial_effects[new_slot_index];
        effect->position.n[0] = position->n[0];
        effect->position.n[1] = position->n[1];
        effect->position.n[2] = position->n[2];
        effect->last_tick = current_tick;
        effect->type = effect_type;
        effect->count = 1;
    }

    if ( should_notify )
        actors_handle_spatial_effect(object_index, effect->type, &effect->position, volume, effect->count);
}
