/* light_fixture_place @0x837E99B0 — places a light-fixture object: links its scenario device information,
 * then copies the tag's color/intensity/falloff/cutoff onto the placed object's runtime light-data fields.
 * Uses the standard salted object-header lookup (see object_type_should_force_baseline_update.c).
 *
 * The runtime light-data fields (+532/+544/+548/+552) are the _light_fixture_datum extension of the
 * object-pool light_fixture_datum (DB-verified). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/scenario_light_fixture_datum.h"
#include "headers/light_fixture_datum.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"


#include "headers/scenario_device_datum.h"
extern void device_add_scenario_information(unsigned int device_index, scenario_device_datum *scenario_device);

void light_fixture_place(int light_fixture_index, scenario_light_fixture_datum *scenario_light_fixture)
{
    light_fixture_datum *object = ((light_fixture_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, light_fixture_index)->datum);

    device_add_scenario_information(light_fixture_index, &scenario_light_fixture->device);

    object->light_fixture.color         = scenario_light_fixture->color;
    object->light_fixture.intensity     = scenario_light_fixture->intensity;
    object->light_fixture.falloff_angle = scenario_light_fixture->falloff_angle;
    object->light_fixture.cutoff_angle  = scenario_light_fixture->cutoff_angle;
}
