/* player_effect_add_continuous_effect @0x83737578 — accumulate a distance-attenuated continuous camera
 * effect (controller vibration + camera shake) for a local player from a player-effect definition. The
 * attenuation ramps linearly between the definition's near and far distances; a periodic function further
 * modulates the shake over time. Beyond the far distance nothing is added.
 *
 * The definition is a continuous_damage_effect_definition ('cdmg'): falloff/cutoff radii, the embedded
 * vibrate (frequency scales) and camera_shake (translation/rotation magnitudes + periodic function). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/player_effect_globals.h"
#include "headers/continuous_damage_effect_definition.h"

extern int game_time_get(void);
extern float periodic_function_evaluate(int16_t function_type, double time);

void player_effect_add_continuous_effect(int16_t local_player_index, int effect_index, float distance)
{
    continuous_damage_effect_definition *definition =
        TAG_GET(continuous_damage_effect_definition, effect_index);
    player_effect_datum *player_effect;
    float near_distance = definition->falloff_radius;
    float far_distance = definition->cutoff_radius;
    float attenuation;
    float modulation;
    float translational;
    float rotational;

    if ( distance >= far_distance )
        return;

    player_effect = &player_effect_globals->local_player_effect_data[local_player_index];

    attenuation = ((float)1.0 - ((distance - near_distance) / (near_distance - far_distance)));
    if ( attenuation < 0.0 )
        attenuation = 0.0f;
    else if ( attenuation > 1.0 )
        attenuation = 1.0f;

    modulation = (((definition->camera_shake.periodic_weight
                    * periodic_function_evaluate(definition->camera_shake.periodic_function,
                          ((float)game_time_get() / definition->camera_shake.periodic_period)))
                    + ((float)1.0 - definition->camera_shake.periodic_weight)) * attenuation);

    if ( player_effect->continuous_effect_timer > 0 )
    {
        player_effect->continuous_effect_timer = 0;
        player_effect->continuous_effect.vibrate_frequencies[0] = 0.0f;
        player_effect->continuous_effect.vibrate_frequencies[1] = 0.0f;
        player_effect->continuous_effect.translational_shake = 0.0f;
        player_effect->continuous_effect.rotational_shake = 0.0f;
    }

    translational = (definition->camera_shake.random_translation_magnitude * modulation);
    if ( translational <= 0.0 )
        translational = 0.0f;
    player_effect->continuous_effect.translational_shake += translational;

    rotational = (definition->camera_shake.random_rotation_magnitude * modulation);
    if ( rotational <= 0.0 )
        rotational = 0.0f;
    player_effect->continuous_effect.rotational_shake += rotational;

    player_effect->continuous_effect.vibrate_frequencies[0] +=
        (definition->vibrate.frequencies[0] * attenuation);
    player_effect->continuous_effect.vibrate_frequencies[1] +=
        (definition->vibrate.frequencies[1] * attenuation);
}
