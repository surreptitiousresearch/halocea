/* hcex_control_toggle @0x836837A8 — HCEX bridge hook fired when a scenario control (e.g. a lift) is
 * toggled. Identifies the control by its scenario object name and, on the c10 map, fires the
 * corresponding HCEX telemetry event: entering the forerunner-facility lift ("lift_a_cont_a") emits
 * "c10_enter_ff" with the current game time, leaving it ("lift_d_cont_a") emits "c10_exit_ff" with the
 * negated game time.
 *
 * DEVIATION: the binary inlines the two name comparisons as open-coded strcmp loops; reproduced here
 * with strcmp. The `value` argument is part of the toggle callback signature but is unused here. */

#include <stdint.h>
#include "../headers/data_array.h"
#include "../headers/scenario.h"
#include "../headers/scenario_object_name.h"
#include "../headers/object_header_datum.h"

extern "C" data_array *object_header_data;
extern "C" scenario   *global_scenario;

extern "C" char *main_get_map_name(void);
extern "C" int game_time_get(void);
extern "C" void        hcex_fire_event_i(const char *event_name, const char *param_name, int value);
extern "C" char       *strstr(const char *haystack, const char *needle);
extern "C" int         strcmp(const char *a, const char *b);

extern "C" void hcex_control_toggle(int id, float value)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;
    int16_t name_index = object->object.name_index;
    if (name_index == -1)
        return;

    const char *name = ((scenario_object_name *)global_scenario->object_names.address)[name_index].name;
    if (!name)
        return;

    if (strcmp(name, "lift_a_cont_a") == 0)
    {
        if (strstr(main_get_map_name(), "c10"))
            hcex_fire_event_i("c10_enter_ff", "time", game_time_get());
    }
    if (strcmp(name, "lift_d_cont_a") == 0)
    {
        if (strstr(main_get_map_name(), "c10"))
            hcex_fire_event_i("c10_exit_ff", "time", -game_time_get());
    }
}
