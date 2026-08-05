/* actor_place @0x83721600 — instantiate the unit object for one actor at a starting location and bind a new
 * actor to it. Resolves the (optionally major-upgraded) actor variant, places the object via object_new, then
 * creates the AI actor. On actor-creation failure the freshly placed object is deleted. Returns the actor
 * index, or -1 on failure. */

#include <stdint.h>
#include "headers/encounter_definition.h"
#include "headers/encounter_flags.h"
#include "headers/squad_definition.h"
#include "headers/scenario.h"
#include "headers/actor_starting_location_definition.h"
#include "headers/object_placement_data.h"
#include "headers/actor_variant_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_default_state.h"
#include "headers/actor_definition_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern void objects_garbage_collection(void);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern int object_new(object_placement_data *data);
extern void actor_customize_unit(int actor_variant_definition_index, int unit_index);
extern int actor_create_for_unit(uint8_t swarm, int unit_index, int actor_variant_definition_index, int encounter_index, int16_t squad_index, uint8_t allow_addition_to_other_squads, int disallow_actor_index, uint8_t initially_braindead, int16_t initial_state, int16_t default_state, int16_t initial_command_list_index, char noncombat_sequence_id);
extern void object_delete(int object_index);

int actor_place(int actor_variant_definition_index, int encounter_index, int16_t squad_index,
                actor_starting_location_definition *starting_location, uint8_t upgrade_major,
                int16_t variant_number)
{
    objects_garbage_collection();

    /* DEVIATION: decompiler dereferences the tag-instance slot (*(void**)TAG_INSTANCE) to reach the
     * definition data; the prior source omitted the deref. Restored. */
    actor_variant_definition *variant_definition = TAG_GET(actor_variant_definition, actor_variant_definition_index);
    if ( upgrade_major )
    {
        actor_variant_definition_index = variant_definition->major_upgrade_reference.index;
        variant_definition = TAG_GET(actor_variant_definition, actor_variant_definition_index);
    }

    /* actor_reference = the AI character definition (read for its flags); unit_reference = object placed */
    int *unit_definition = TAG_GET(int, variant_definition->actor_reference.index);

    object_placement_data placement;
    object_placement_data_new(&placement, variant_definition->unit_reference.index, -1);
    placement.position.n[0] = starting_location->position.n[0];
    placement.position.n[1] = starting_location->position.n[1];
    placement.position.n[2] = starting_location->position.n[2];
    vector3d_from_angle(&placement.forward, starting_location->facing);
    placement.variant_number = variant_number;

    int unit_index = object_new(&placement);
    if ( unit_index == -1 )
        return -1;

    uint8_t swarm = (*(unsigned int *)unit_definition >> _actor_definition_swarm_actor_bit) & 1;

    actor_customize_unit(actor_variant_definition_index, unit_index);

    /* States default to the squad's, overridden by any positive value on the starting location. The deviation
     * from the decompiler (which dropped these computations) is recovered from the call-site disassembly. */
    uint8_t initially_braindead = 0;
    int16_t initial_state = actor_default_state_none;
    int16_t default_state = actor_default_state_none;
    if ( encounter_index != -1 )
    {
        encounter_definition *encounter_def = (encounter_definition *)global_scenario->ai_encounters.address + (uint16_t)encounter_index;
        /* disasm 83721708: *((_DWORD *)ai_encounters.address + 44*idx + 8) — record stride 44 dwords
         * (176 bytes), so flags is dword 8 = byte 32 = encounter_def->flags; braindead = bit 4 (0x10).
         * The prior byte-8 read (dword+2) was a mis-scale — the +8 is a dword offset, not a byte offset. */
        initially_braindead = (encounter_def->flags >> _encounter_braindead_bit) & 1;
        squad_definition *squad = (squad_definition *)encounter_def->squads.address + squad_index;
        initial_state = squad->initial_state;
        default_state = squad->default_state;
    }
    if ( starting_location->initial_state > actor_default_state_none )
        initial_state = starting_location->initial_state;
    if ( starting_location->default_state > actor_default_state_none )
        default_state = starting_location->default_state;

    int actor_index = actor_create_for_unit(swarm, unit_index, actor_variant_definition_index, encounter_index,
                                            squad_index, 0, -1, initially_braindead, initial_state, default_state,
                                            starting_location->command_list_index, starting_location->sequence_id);
    if ( actor_index == -1 )
        object_delete(unit_index);
    return actor_index;
}
