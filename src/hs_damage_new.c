/* hs_damage_new @0x837F8680 — apply an area-of-effect damage at a scenario cutscene flag's location. Builds
 * a damage_data from the damage-effect definition, sets its origin/epicenter to the flag position (cutscene
 * flag +0x24), resolves the location, and applies the damage with no instigator. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_cutscene_flag.h"
#include "headers/damage_data.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void area_of_effect_cause_damage(damage_data *damage_data, int unlucky_object_index);

void hs_damage_new(int definition_index, int16_t flag_index)
{
    const real_point3d *flag_position =
        &((scenario_cutscene_flag *)global_scenario->cutscene_flags.address)[flag_index].position;

    damage_data damage;
    damage_data_new(&damage, definition_index);
    damage.epicenter = *flag_position;
    damage.origin = *flag_position;
    scenario_location_from_point(&damage.location, flag_position);
    area_of_effect_cause_damage(&damage, -1);
}
