/* actor_move_animation_impulse @0x837C89C8 — give an actor a movement-animation impulse (e.g. dodge),
 * recording the impulse id (word @ actor +1048) and 2D alignment (dwords @ actor +1052/+1056). Wakes
 * the actor from dormancy; fails (returns 0) if a movement animation is already busy. Stride 1828. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/real_vector2d.h"
#include "headers/blam_data_globals.h"

extern void actor_set_dormant(int actor_index, uint8_t dormant);
extern uint8_t actor_move_animation_busy(int actor_index);

uint8_t actor_move_animation_impulse(int actor_index, int16_t animation_impulse, const real_vector2d *animation_alignment)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_set_dormant(actor_index, 0);
    if ( actor_move_animation_busy(actor_index) )
        return 0;
    actor->orders.move.animation.impulse = animation_impulse;
    actor->orders.move.animation.alignment_vector.n[0] = animation_alignment->n[0];
    actor->orders.move.animation.alignment_vector.n[1] = animation_alignment->n[1];
    return 1;
}
