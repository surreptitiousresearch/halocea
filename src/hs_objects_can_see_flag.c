/* hs_objects_can_see_flag @0x837F7AC8 — return true if any unit in the object list can see a scenario
 * cutscene flag's position (flag +0x24) within the given cone half-angle (degrees). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_cutscene_flag.h"
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern int object_list_get_first(int object_list_index, int *iterator);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t unit_can_see_point(int unit_index, const real_point3d *point, float theta);

uint8_t hs_objects_can_see_flag(int object_list_index, int16_t flag_index, float degrees)
{
    int iterator[4];
    int unit_index = object_list_get_first(object_list_index, iterator);
    if ( unit_index == -1 )
        return 0;

    while ( 1 )
    {
        if ( object_try_and_get_and_verify_type(unit_index, object_mask_unit) )
        {
            unsigned char can_see = 0;
            if ( flag_index )
                can_see = unit_can_see_point(unit_index,
                              &((scenario_cutscene_flag *)global_scenario->cutscene_flags.address)[flag_index].position,
                              degrees * DEG_TO_RAD);
            if ( can_see )
                break;
        }
        unit_index = object_list_get_next(object_list_index, iterator);
        if ( unit_index == -1 )
            return 0;
    }
    return 1;
}
