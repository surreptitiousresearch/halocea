/* biped_new_from_network @0x837AD9D8 — (client) apply a "biped spawn" message-delta: decode the biped's
 * definition, owner team/player/object (as translated network indices), placement transform, translational
 * velocity, change colors, and animation/movement prediction seed, then create a puppet biped at that transform
 * and prime its client-side prediction state. The decoded up vector is re-orthogonalized against forward
 * (up = forward x (up x forward)) and both are normalized before placement. The player and owner-object indices
 * are mapped from network-translated to local; the fresh object's translated index is registered. After creation
 * the biped's prediction block (@+1318) is seeded from the message and mirrored into the live animation/movement
 * fields (a scaled value @+228, aiming @+224, a state flag @+260, a copied 12-byte block +1172->+1196, etc.).
 * No-op for incremental-mode headers.
 *
 * Same decode/reorthogonalize/register/OBJECT_DATA idioms as the other *_new_from_network spawns; unlike them the
 * placement also carries translational_velocity and change_colors (consumed by object_new). DEVIATION: the
 * placement zero-init the decompiler rendered as a __PAIR64__(owner_player_index, 0) store loop is a plain
 * memset-to-0 (confirmed r9=0 in the projectile sibling); the decode destination is one contiguous buffer
 * reproduced as a single struct. Prediction/live fields are raw object offsets per the net-delta convention. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/object_placement_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/field_properties_definition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/networked_datum_role.h"
#include "headers/biped_spawn_message.h"
#include "headers/biped_datum.h"
#include <string.h>
#include "headers/blam_data_globals.h"
#include "headers/unit_grenade_type.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern float normalize3d(real_vector3d *v);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role);
extern uint8_t field_translated_index_client_register_local_and_translated_index(const _field_properties_definition *const field_properties_definition, const int local_index, const int translated_index);


void biped_new_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _biped_spawn_message message;
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
    placement.translational_velocity = message.translational_velocity;
    placement.forward = forward;
    placement.up = up;
    memcpy(placement.change_colors, message.change_colors, sizeof(placement.change_colors));

    int object_index = object_new_with_datum_role_control(&placement, _networked_datum_puppet);
    if ( object_index == -1 )
        return;

    field_translated_index_client_register_local_and_translated_index(&field_properties_object_index_definition,
            object_index, message.server_object_index);

    biped_datum *biped = (biped_datum *)((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);

    biped->biped.baseline.body_vitality = message.body_vitality;
    biped->biped.baseline.shield_vitality = message.shield_vitality;
    biped->biped.baseline.shield_stun_ticks_greater_than_zero = message.shield_stun_ticks_greater_than_zero;
    /* DEVIATION: was a 16-bit pun copy across both bytes of char[2]; element-wise copy is identical */
    biped->biped.baseline.grenade_counts[0] = message.grenade_counts[0];
    biped->biped.baseline.grenade_counts[1] = message.grenade_counts[1];

    uint8_t predicted_flag = biped->biped.baseline.shield_stun_ticks_greater_than_zero;
    float predicted_value = biped->biped.baseline.body_vitality;
    char state_801 = biped->unit.desired_zoom_level;
    float movement_block0 = biped->unit.___u87.most_recent_control_data_for_network.facing_vector.n[0];
    float movement_block1 = biped->unit.___u87.most_recent_control_data_for_network.facing_vector.n[1];
    /* DEVIATION: 16-bit pun copy baseline→unit grenade_counts untangled to per-byte locals */
    char predicted_grenades0 = biped->biped.baseline.grenade_counts[0];
    char predicted_grenades1 = biped->biped.baseline.grenade_counts[1];

    biped->object.shield_vitality = biped->biped.baseline.shield_vitality * 3.0f;
    biped->biped.baseline_index = message.current_baseline_index;
    float movement_block2 = biped->unit.___u87.most_recent_control_data_for_network.facing_vector.n[2];
    biped->object.body_vitality = predicted_value;
    biped->biped.baseline_valid = 1;
    biped->biped.message_index = 0;
    biped->unit.___u87.most_recent_control_data_for_network.zoom_level = state_801;
    biped->object.shield_stun_ticks = (predicted_flag == 1);
    biped->unit.___u87.most_recent_control_data_for_network.looking_vector.n[0] = movement_block0;
    biped->unit.___u87.most_recent_control_data_for_network.looking_vector.n[1] = movement_block1;
    biped->unit.___u87.most_recent_control_data_for_network.looking_vector.n[2] = movement_block2;
    biped->unit.grenade_counts[_unit_grenade_human_fragmentation] = predicted_grenades0;
    biped->unit.grenade_counts[_unit_grenade_covenant_plasma] = predicted_grenades1;
    biped->unit.is_from_network_data_valid = 1;
}
