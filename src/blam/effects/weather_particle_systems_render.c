/* weather_particle_systems_render @0x8373D838 — update and render the local player's weather particle
 * system. Refreshes the system's location from the render camera, resolves the active weather definition
 * from the BSP's weather palette for the camera's fog region, creates/deletes the system if the definition
 * changed, and renders it when present. No-op when weather is off or no local player is rendering. */

#include <stdint.h>
#include "headers/weather_particle_system_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_bsp.h"
#include "headers/structure_weather_palette_entry.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);
extern void weather_particle_system_delete(int16_t system_index);
extern void weather_particle_system_new(int16_t system_index, int definition_index, float scale);
extern void weather_particle_system_render(int16_t system_index);

void weather_particle_systems_render(void)
{
    if (!weather || render.local_player_index == -1)
        return;

    weather_particle_system_datum *system = &weather_particle_system_globals.systems[render.local_player_index];

    system->location.cluster_index = render.cluster_index;
    system->location.leaf_index = render.leaf_index;
    system->under_water = scenario_location_underwater(&system->location, &render.camera.position,
                                                       &system->weather_palette_index);

    int definition_index = -1;
    if (system->weather_palette_index != -1)
        definition_index = ((structure_weather_palette_entry *)global_structure_bsp->weather_palette.address)[system->weather_palette_index].particle_system.index;

    if (system->definition_index != definition_index)
    {
        if (system->definition_index != -1)
            weather_particle_system_delete(render.local_player_index);
        if (definition_index != -1)
            weather_particle_system_new(render.local_player_index, definition_index, 1.0f);
    }

    if (system->definition_index != -1)
        weather_particle_system_render(render.local_player_index);
}
