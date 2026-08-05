#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_cutscene_flag.h"
#include "headers/real_point3d.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_can_see_point(int unit_index, const real_point3d *point, float theta);

uint8_t hs_unit_can_see_flag(int unit_index, int16_t flag_index, float degrees)
{
    if (!flag_index)
        return 0;

    const real_point3d *flag_position =
        &((scenario_cutscene_flag *)global_scenario->cutscene_flags.address)[flag_index].position;

    return unit_can_see_point(unit_index, flag_position, degrees * DEG_TO_RAD);
}
