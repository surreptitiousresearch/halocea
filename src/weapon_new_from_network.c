/* weapon_new_from_network @0x836DA700 — (client) apply a "weapon spawn" message-delta: decode the weapon's
 * definition, owner team/player/object (as translated network indices), placement transform, object flags, and
 * client-prediction seed, then create a puppet weapon object at that transform and prime its prediction state.
 * The decoded up vector is re-orthogonalized against forward (up = forward x (up x forward)) and both are
 * normalized before placement. The player and owner-object indices are mapped from network-translated to local
 * indices; the fresh object's translated index is registered. After creation the decoded flags are OR'd into the
 * object flags (@+16), the position and linear velocity are written into the weapon's gearbox prediction block
 * (control @+736, position @+740, linear velocity @+752) and applied (gearbox_object_translate), then the
 * predicted values are mirrored into the object's live fields (linear velocity @+104, plus the weapon-specific
 * short/float fields @+576/+694/+706/+696/+708). No-op for incremental-mode headers.
 *
 * Structurally identical to equipment_new_from_network / projectile_new_from_network (same decode/placement/
 * register/OBJECT_DATA idioms); only the object prediction block base (+736 vs +580) and the weapon-specific
 * prediction tail differ. DEVIATION: the placement zero-init the decompiler rendered as a
 * __PAIR64__(owner_player_index, 0) store loop is a plain memset-to-0 (confirmed r9=0 in the sibling); the
 * decoder's by-ref destination is one contiguous buffer reproduced as a single struct. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/object_placement_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/field_properties_definition.h"
#include "headers/data_array.h"
#include "headers/networked_datum_role.h"
#include "headers/weapon_spawn_message.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include <string.h>
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern float normalize3d(real_vector3d *v);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role);
extern uint8_t field_translated_index_client_register_local_and_translated_index(const _field_properties_definition *const field_properties_definition, const int local_index, const int translated_index);
extern void gearbox_object_translate(int object_index, const real_point3d *new_position);

void weapon_new_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _weapon_spawn_message message;
    if ( message_delta_processor_decode_stateless(&message, header) != 1 )
        return;

    /* re-orthogonalize up against forward: up = forward x (up x forward), then normalize both */
    real_vector3d forward = message.forward;
    real_vector3d up_in = message.up;
    float a = forward.n[0] * up_in.n[1] - up_in.n[0] * forward.n[1];
    float b = up_in.n[2] * forward.n[1] - forward.n[2] * up_in.n[1];
    float c = up_in.n[0] * forward.n[2] - up_in.n[2] * forward.n[0];
    real_vector3d up;
    up.n[0] = c * forward.n[2] - a * forward.n[1];
    up.n[1] = a * forward.n[0] - b * forward.n[2];
    up.n[2] = b * forward.n[1] - c * forward.n[0];
    normalize3d(&forward);
    normalize3d(&up);

    int owner_object_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
            message.owner_object_index);
    int owner_player_index = field_translated_index_get_local_index(&field_properties_player_index_definition,
            message.owner_player_index);

    object_placement_data placement;
    memset(&placement, 0, sizeof(placement));
    placement.definition_index = message.definition_index;
    placement.owner_team_index = message.owner_team_index;
    placement.owner_player_index = owner_player_index;
    placement.owner_object_index = owner_object_index;
    placement.position = message.position;
    placement.forward = forward;
    placement.up = up;

    int object_index = object_new_with_datum_role_control(&placement, _networked_datum_puppet);
    if ( object_index == -1 )
        return;

    field_translated_index_client_register_local_and_translated_index(&field_properties_object_index_definition,
            object_index, message.server_object_index);

    weapon_datum *object =
        (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    object->object.flags |= message.object_flags;
    object->weapon.baseline.position.n[0] = message.position.n[0];
    object->weapon.baseline.position.n[1] = message.position.n[1];
    object->weapon.baseline.position.n[2] = message.position.n[2];
    object->weapon.baseline.translational_velocity.n[0] = message.translational_velocity.n[0];
    object->weapon.baseline.translational_velocity.n[1] = message.translational_velocity.n[1];
    object->weapon.baseline.translational_velocity.n[2] = message.translational_velocity.n[2];
    object->weapon.baseline.magazine_0_rounds_total = message.magazine_0_rounds_total;
    object->weapon.baseline.magazine_1_rounds_total = message.magazine_1_rounds_total;
    object->weapon.baseline.age = message.age;
    object->weapon.baseline_index = message.current_baseline_index;
    object->weapon.baseline_valid = 1;
    object->weapon.message_index = 0;
    gearbox_object_translate(object_index, &object->weapon.baseline.position);
    object->weapon.age = object->weapon.baseline.age;
    *(int *)&object->object.translational_velocity.n[0] = *(int *)&object->weapon.baseline.translational_velocity.n[0];
    *(int *)&object->object.translational_velocity.n[1] = *(int *)&object->weapon.baseline.translational_velocity.n[1];
    *(int *)&object->object.translational_velocity.n[2] = *(int *)&object->weapon.baseline.translational_velocity.n[2];
    object->weapon.magazines[0].rounds_total = object->weapon.baseline.magazine_0_rounds_total;
    object->weapon.magazines[1].rounds_total = object->weapon.baseline.magazine_1_rounds_total;
    object->weapon.magazines[0].rounds_loaded = message.magazine_0_rounds_loaded;
    object->weapon.magazines[1].rounds_loaded = message.magazine_1_rounds_loaded;
}
