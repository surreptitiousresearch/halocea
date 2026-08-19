/* damage_effect_new_on_object @0x836B2198 */
/* DEVIATION: prior 6-arg signature was a decompiler phantom — the 8-instruction thunk reads
 * only r3/r4 (0x836B219C mr r5,r4; 0x836B2200 li r6,-1) and hard-codes NULL for both the
 * color (0x836B21A4 li r9,0) and impulse_field (0x836B21A0 li r10,0) slots; node_index/flags/
 * tint_color/vector_field params do not exist in the binary. */
#include <stdint.h>
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"

extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);

void damage_effect_new_on_object(int effect_definition_index, int object_index)
{
    effect_new_from_object(effect_definition_index, object_index, object_index, -1, 0.0f, 0.0f, 0, 0);
}
