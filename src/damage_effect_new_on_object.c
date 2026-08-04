#include <stdint.h>
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"

extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);

void damage_effect_new_on_object(int effect_definition_index, int object_index, int node_index, int flags,
                                  const real_rgb_color *tint_color, const effect_vector_field *vector_field)
{
    effect_new_from_object(effect_definition_index, object_index, object_index, -1, 0.0f, 0.0f, tint_color, vector_field);
}
