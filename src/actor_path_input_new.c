/* actor_path_input_new @0x837C8798 — builds a fresh path_input for an actor: pathfinding radius defaults
 * to the actor definition's tag value, but if the actor is driving a vehicle (vehicle_driver_type > 0) and
 * that vehicle's own tag exposes a larger pathfinding radius, uses that instead. Also tells the pathfinder
 * which object to ignore for collision — the vehicle when the actor is driving one, otherwise the actor's
 * own unit — and seeds the start point/surface from the actor's last-known pathfinding position.
 *
 * DEVIATION: disasm_range(0x837C87C8, 0x837C8880) resolves two decompiler ambiguities: (1) the decompile's
 * path_input_new call passes an unassigned local as its 3rd argument — disasm shows the real 3rd
 * argument (r5) is the byte at actor+886 and the real 4th argument (r6) is a two-path select the decompile
 * dropped entirely (r4 is a dead shadow GPR for the float radius argument, which shifts the following
 * GPR-argument numbering). The select lives in nonvolatile r30: `lwz r30, 0x18(r31)` @0x837C87D8 preloads
 * actor->meta.unit_index (actor+0x18) unconditionally, and only inside the vehicle_driver_type>0 branch do
 * `lwz r11, 0x158(r31)` @0x837C87F8 + `mr r30, r11` @0x837C8808 overwrite it with input.vehicle_index —
 * before, and independent of, the vehicle-radius compare; `mr r6, r30` @0x837C884C passes the survivor.
 * There is no NONE guard downstream: obstacles_get_discs_in_sphere indexes object_header_data with this
 * value at its first instruction (`clrlslwi r11, r7, 16,1` @0x8382FA5C, `lwz r11, 8(r7)` @0x8382FAA0);
 * (2) the decompile's `(const real_point3d *)actor + 30`
 * start-point argument is confirmed to mean actor+0x168 (cast-then-scale, not a raw byte offset), matching
 * actor_input_data.pathfinding_point exactly. actor+886 is emotions.ignorant_of_broken_surfaces. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/vehicle_definition.h"
#include "headers/path_input.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


extern void actor_find_pathfinding_location(int actor_index);
extern void path_input_new(path_input *input, float pathfinding_radius, uint8_t ignore_broken_surfaces, int ignore_source_object_index);
extern void path_input_set_start(path_input *input, const real_point3d *start_point, int start_surface_index);

void actor_path_input_new(int actor_index, path_input *input)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);
    float pathfinding_radius = definition->moving.pathfinding_radius;
    int source_object_index = actor->meta.unit_index;

    if (actor->input.vehicle_driver_type > 0)
    {
        int vehicle_index = actor->input.vehicle_index;
        source_object_index = vehicle_index;
        char *vehicle = ((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
        vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, *(int *)vehicle);
        float vehicle_radius = vehicle_def->vehicle.ai_pathfinding_radius;
        if (vehicle_radius > 0.0f)
            pathfinding_radius = vehicle_radius;
    }

    actor_find_pathfinding_location(actor_index);

    /* recovered: raw ((unsigned char *)actor)[886] -> emotions.ignorant_of_broken_surfaces */
    unsigned char ignore_broken_surfaces = actor->emotions.ignorant_of_broken_surfaces;
    path_input_new(input, pathfinding_radius, ignore_broken_surfaces, source_object_index);
    path_input_set_start(input, &actor->input.pathfinding_point, actor->input.pathfinding_surface_index);
}
