/* actor_path_input_new @0x837C8798 — builds a fresh path_input for an actor: pathfinding radius defaults
 * to the actor definition's tag value, but if the actor is driving a vehicle (vehicle_driver_type > 0) and
 * that vehicle's own tag exposes a larger pathfinding radius, uses that instead. Also tells the pathfinder
 * to ignore collision with the actor's own vehicle (if any) and seeds the start point/surface from the
 * actor's last-known pathfinding position.
 *
 * DEVIATION: disasm_range(0x837C87C8, 0x837C8880) resolves two decompiler ambiguities: (1) the decompile's
 * `path_input_new(input, v4, v6, byte@actor+886)` call has an unassigned "v6" — disasm shows the real 3rd
 * argument (r5) is the byte at actor+886 and the real 4th argument (r6) is actor->input.vehicle_index, the
 * reverse of the decompile's textual order (r4 is a dead shadow GPR for the float radius argument, which
 * shifts the following GPR-argument numbering); (2) `(const real_point3d *)v3 + 30` is confirmed to mean
 * actor+0x168 (cast-then-scale, not a raw byte offset), matching actor_input_data.pathfinding_point
 * exactly. The byte at actor+886 is actor_datum's emotions.ignorant_of_broken_surfaces. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/vehicle_definition.h"
#include "headers/path_input.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


extern void actor_find_pathfinding_location(uint16_t actor_index);
extern void path_input_new(path_input *input, float pathfinding_radius, uint8_t ignore_broken_surfaces, int ignore_source_object_index);
extern void path_input_set_start(path_input *input, const real_point3d *start_point, int start_surface_index);

void actor_path_input_new(uint16_t actor_index, path_input *input)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);
    float pathfinding_radius = definition->moving.pathfinding_radius;

    if (actor->input.vehicle_driver_type > 0)
    {
        int vehicle_index = actor->input.vehicle_index;
        char *vehicle = ((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
        vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, *(int *)vehicle);
        float vehicle_radius = vehicle_def->vehicle.ai_pathfinding_radius;
        if (vehicle_radius > 0.0f)
            pathfinding_radius = vehicle_radius;
    }

    actor_find_pathfinding_location(actor_index);

    /* recovered: raw ((unsigned char *)actor)[886] -> emotions.ignorant_of_broken_surfaces */
    unsigned char ignore_broken_surfaces = actor->emotions.ignorant_of_broken_surfaces;
    path_input_new(input, pathfinding_radius, ignore_broken_surfaces, actor->input.vehicle_index);
    path_input_set_start(input, &actor->input.pathfinding_point, actor->input.pathfinding_surface_index);
}
