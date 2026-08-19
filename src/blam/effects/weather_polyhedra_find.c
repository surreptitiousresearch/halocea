/* weather_polyhedra_find @0x8373C750 — collect up to 8 indices of weather bounding-polyhedra (32-byte
 * records: center point + radius @+12) whose sphere, expanded by `maximum_distance`, contains the camera.
 * Returns how many indices were written. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_weather_polyhedron.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"


int16_t weather_polyhedra_find(int16_t *weather_polyhedra_indices, float maximum_distance)
{
    int16_t found = 0;

    for ( int16_t i = 0; i < global_structure_bsp->weather_polyhedra.count; ++i )
    {
        structure_weather_polyhedron *polyhedron =
            &((structure_weather_polyhedron *)global_structure_bsp->weather_polyhedra.address)[i];
        float radius = polyhedron->bounding_sphere_radius + maximum_distance;

        float dx = polyhedron->bounding_sphere_center.n[0] - render.camera.position.n[0];
        float dy = polyhedron->bounding_sphere_center.n[1] - render.camera.position.n[1];
        float dz = polyhedron->bounding_sphere_center.n[2] - render.camera.position.n[2];

        if ( dx * dx + dy * dy + dz * dz < radius * radius )
        {
            if ( found < 8 )
                weather_polyhedra_indices[found++] = i;
        }
    }

    return found;
}
