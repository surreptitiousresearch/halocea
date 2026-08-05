/* ai_consider_major_upgrade @0x836E77F0 — error-diffused random roll deciding whether a placed actor receives
 * the major upgrade. The accumulated rounding error is fed back into both the squad and the global accumulator
 * so the long-run upgrade rate converges on upgrade_chance. Returns true when this actor is upgraded. */

#include "headers/squad_datum.h"
#include "headers/ai_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

uint8_t ai_consider_major_upgrade(int encounter_index, int16_t squad_index, float upgrade_chance)
{
    /* Encounter datum (stride 108): word at offset 4 = first squad index in squad_array. */
    squad_datum *squad = &squad_array[(int16_t)(((int16_t *)encounter_data->data)[54 * (uint16_t)encounter_index + 2] + squad_index)];

    float squad_error = squad->major_upgrade_error;

    /* Bias the threshold toward the squad's outstanding error, clamped to a third of the global error. */
    float global_bias = ai_globals->major_upgrade_error * -0.33333334f;
    float bias;
    if ( __fabs(global_bias) <= __fabs(-squad_error) )
        bias = -squad_error;
    else
        bias = global_bias;

    unsigned int *seed = get_global_random_seed_address();
    uint8_t result = real_seed_random(seed) < (bias + upgrade_chance);

    float error = (float)result - upgrade_chance;
    squad->major_upgrade_error += error;
    ai_globals->major_upgrade_error += error;
    return result;
}
