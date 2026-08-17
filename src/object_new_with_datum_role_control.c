/* object_new_with_datum_role_control @0x836F16C8 — the core object spawn routine. Allocates an object
 * header datum plus its variable-size game-datum block, initializes the full object data record from
 * the placement request (position/orientation/velocity, owner, variant, flags, default colors), runs
 * the per-type new chain, connects the object to the map, and — on a network server — broadcasts the
 * spawn as a delta message. On any failure it tears everything down, prints "OUT OF OBJECTS", and
 * returns -1; otherwise returns the new object index (after firing the definition's creation effect).
 *
 * Deviations:
 *  - The trailing effect_new_from_object call passes NULL color and NULL impulse_field (r9=r10=0 at
 *    0x836F1CBC-0x836F1CC0); the decompiler's v25/v26 args are phantoms. v25 is really a local object-
 *    type bitmask used only in the node-block allocation branch, not an effect argument. */

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "headers/object_type.h"
#include "headers/real_matrix4x3.h"
#include "headers/object_placement_data.h"
#include "headers/new_object_data_flags.h"
#include "headers/networked_datum_role.h"
#include "headers/object_type_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/model.h"
#include "headers/game_connection.h"
#include "headers/object_flags.h"
#include "headers/object_definition_flags.h"
#include "headers/object_header_flags.h"
#include "headers/effect_vector_field.h"
#include "headers/blam_data_globals.h"

#include "headers/real_rgb_color.h"
#include "headers/data_array.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/network_message_type.h"

typedef struct network_game_server network_game_server;

extern int sprintf_0(char *string, const char *format, ...);
extern uint8_t game_engine_running(void);
extern int16_t game_connection(void);
extern int game_engine_remap_object_definition(int definition_index);
extern object_type_definition *object_type_definition_get(int16_t object_type);
extern int object_header_new(data_array *data, int index, int16_t size);
extern void object_type_adjust_placement(int object_index, object_placement_data *data);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern uint8_t object_header_block_allocate(int object_index, int16_t block_reference_offset, int16_t size);
extern uint8_t object_type_new(int object_index);
extern void object_choose_random_change_colors(int object_index, const real_rgb_color *placement_change_colors);
extern void object_choose_random_region_permutations(int object_index);
extern void object_initialize_vitality(int object_index, const float *custom_body_vitality, const float *custom_shield_vitality);
extern void object_compute_node_matrices(int object_index);
extern void object_reconnect_to_map(int object_index, const location *location_in);
extern void object_postprocess_node_matrices(int object_index);
extern void object_type_export_function_values(int object_index);
extern void object_compute_function_values(int object_index);
extern void object_compute_change_colors(int object_index);
extern void hcex_pix_begin_event(const char *fmt, ...); /* DEVIATION: variadic (std r4..r10 save-area prologue @0x823CED10) */
extern void hcex_pix_end_event(void);
extern void hcex_obj_init(int id);
extern void widgets_new(int object_index);
extern void attachments_new(int object_index);
extern void object_delete(int object_index);
extern void object_type_update_baseline(int object_index);
extern int object_type_new_to_network(int object_index, void *buffer, int buffer_size_in_bits);
extern void object_type_delete(int object_index);
extern void object_header_delete(data_array *data, int object_index);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern char *tag_get_name(int tag_index);
extern const char *tag_name_strip_path(const char *name);
extern void console_printf(uint8_t clear, const char *format, ...);

int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role)
{
    int definition_index = data->definition_index;
    char message[624];

    if ( game_engine_running() )
    {
        if ( definition_index == -1 )
            return -1;
        if ( datum_role != _networked_datum_puppet
            && datum_role != _networked_datum_puppet_controlled_by_local_player )
            definition_index = game_engine_remap_object_definition(definition_index);
    }
    if ( definition_index == -1 )
        return -1;

    _object_definition *object_definition = TAG_GET(_object_definition, definition_index);
    int16_t object_type = object_definition->type;
    int16_t game_datum_size = object_type_definition_get(object_type)->game_datum_size;

    /* DEVIATION: the decompiler's datum_new + pool-allocate + memset block here is a verbatim
     * compiler-inlined copy of object_header_new @0x836ECD98 constant-folded for index=-1
     * (already re-sourced; body matches exactly) — emitted as the call the source had. */
    int object_index = object_header_new(object_header_data, -1, game_datum_size);
    if ( object_index == -1 )
        goto out_of_objects;

    char success = 1;
    object_header_datum *header_entry = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object_data = header_entry->datum;

    header_entry->flags |= (1u << _object_header_being_created_bit) | (1u << _object_header_automatically_deactivate_bit);
    header_entry->type = (uint8_t)object_type;
    object_data->definition_index = definition_index;
    object_data->object.type = object_type;
    object_type_adjust_placement(object_index, data);

    object_data->object.scenery_idx = global_object_scenario_index;
    global_object_scenario_index = 0;
    object_data->object.datum_role = datum_role;
    object_data->object.is_server_position_valid = 0;
    object_data->object.actor_variant_definition = -1;

    object_data->object.position.n[0] = data->position.n[0];
    object_data->object.position.n[1] = data->position.n[1];
    object_data->object.position.n[2] = data->position.n[2];
    object_data->object.forward.n[0] = data->forward.n[0];
    object_data->object.forward.n[1] = data->forward.n[1];
    object_data->object.forward.n[2] = data->forward.n[2];
    object_data->object.up.n[0] = data->up.n[0];
    object_data->object.up.n[1] = data->up.n[1];
    object_data->object.up.n[2] = data->up.n[2];
    object_data->object.translational_velocity.n[0] = data->translational_velocity.n[0];
    object_data->object.translational_velocity.n[1] = data->translational_velocity.n[1];
    object_data->object.translational_velocity.n[2] = data->translational_velocity.n[2];
    object_data->object.angular_velocity.n[0] = data->angular_velocity.n[0];
    object_data->object.angular_velocity.n[1] = data->angular_velocity.n[1];
    object_data->object.angular_velocity.n[2] = data->angular_velocity.n[2];

    /* offset the origin along the up vector by the requested height */
    float height = data->height;
    object_data->object.position.n[0] += object_data->object.up.n[0] * height;
    object_data->object.position.n[1] += object_data->object.up.n[1] * height;
    object_data->object.position.n[2] += object_data->object.up.n[2] * height;

    if ( (data->flags & (1u << _new_object_mirrored_bit)) != 0 )
        object_data->object.flags |= (1u << _object_mirrored_bit);
    else
        object_data->object.flags &= ~(1u << _object_mirrored_bit);

    object_data->object.location.cluster_index = -1;
    header_entry->cluster_index = -1;
    object_data->object.umbrella_shield_object_index = -1;
    object_data->object.first_cluster_reference_index = -1;
    object_data->object.animation.state.index = -1;
    object_data->object.magic_number = global_object_marker - 1;
    object_data->object.animation.animation_graph_index = object_definition->animation_graph.index;
    object_data->object.cached_render_state_index = -1;
    object_data->object.parent_object_index = -1;
    object_data->object.next_object_index = -1;
    object_data->object.first_child_object_index = -1;
    object_data->object.name_index = -1;
    object_data->object.shield_damage_decay_timer = -1;
    object_data->object.body_damage_decay_timer = -1;

    if ( (object_definition->flags & (1u << _object_does_not_cast_shadow_bit)) != 0 )
        object_data->object.flags |= (1u << _object_shadowless_bit);

    if ( object_definition->collision_model.index == -1 )
        object_data->object.flags &= ~(1u << _object_has_collision_model_bit);
    else
        object_data->object.flags |= (1u << _object_has_collision_model_bit);

    /* DEVIATION: decompiler rendered the addi/addic/subfe (x+1 != 0) idiom on the full lwz of
     * model.index as a bogus byte-read expression; it is simply (model.index != -1). */
    object_set_visibility(object_index, object_definition->model.index != -1);

    object_data->object.owner_team_index = data->owner_team_index;
    object_data->object.owner_player_index = data->owner_player_index;
    object_data->object.owner_object_index = data->owner_object_index;
    object_data->object.variant_number = data->variant_number;
    object_data->object.forced_shader_permutation_index = object_definition->forced_shader_permutation_index;

    /* node count from the object's model tag for matrix-block allocation */
    int model_index = object_definition->model.index;
    int16_t node_count = (model_index == -1) ? 1 : (int16_t)(TAG_GET(model, model_index))->nodes.count;

    /* Types that keep only the node-matrix block (no separate node-orientation blocks):
     * projectile..sound_scenery. Mask 0xFE0 = bits [projectile..sound_scenery]. */
    int needs_single_node_block = (1 << object_type) &
        ( (1 << object_type_projectile) | (1 << object_type_scenery) | (1 << object_type_machine)
        | (1 << object_type_control) | (1 << object_type_light_fixture)
        | (1 << object_type_placeholder) | (1 << object_type_sound_scenery) );
    if ( !object_header_block_allocate(object_index, offsetof(object_datum, object.node_matrices), sizeof(real_matrix4x3) * node_count)
        || (needs_single_node_block == 0
            && (!object_header_block_allocate(object_index, offsetof(object_datum, object.node_orientations), 32 * node_count)
                || !object_header_block_allocate(object_index, offsetof(object_datum, object.original_node_orientations), 32 * node_count))) )
        success = 0;

    object_data = header_entry->datum;  /* re-fetch: block allocation reallocates the pool block */

    if ( success && object_type_new(object_index) )
    {
        int placement_valid = (object_data->object.flags >> _object_deleted_when_deactivated_bit) & 1;
        if ( placement_valid && (data->flags & (1u << _new_object_never_automatically_delete_bit)) != 0 )
            object_data->object.flags &= ~(1u << _object_deleted_when_deactivated_bit);

        object_choose_random_change_colors(object_index, data->change_colors);
        object_choose_random_region_permutations(object_index);
        object_initialize_vitality(object_index, 0, 0);
        object_compute_node_matrices(object_index);
        object_reconnect_to_map(object_index, 0);
        object_postprocess_node_matrices(object_index);
        object_type_export_function_values(object_index);
        object_compute_function_values(object_index);
        object_compute_change_colors(object_index);
        hcex_pix_begin_event("hcex_obj_init");
        hcex_obj_init(object_index);
        hcex_pix_end_event();
        widgets_new(object_index);
        attachments_new(object_index);

        if ( placement_valid )
            object_data->object.flags |= (1u << _object_deleted_when_deactivated_bit);
        else
            object_data->object.flags &= ~(1u << _object_deleted_when_deactivated_bit);

        if ( (header_entry->flags & (1u << _object_header_active_bit)) == 0
            && (object_data->object.flags & (1u << _object_deleted_when_deactivated_bit)) != 0
            && ((data->flags & (1u << _new_object_never_automatically_delete_bit)) == 0 || (uint16_t)object_data->object.location.cluster_index != 0xFFFF) )
            object_delete(object_index);
    }
    else
    {
        success = 0;
    }

    if ( !gIsClientVersion && success == 1 && game_connection() == _game_connection_network_server && !object_data->object.datum_role )
    {
        object_type_update_baseline(object_index);
        int message_size = object_type_new_to_network(object_index, g_message_encode_buffer, 32760);
        if ( message_size > 0 )
        {
            network_game_server *server = global_network_game_server_get();
            network_game_server_send_message_to_all_loaded_machines(server,
                network_message_type_message_delta, g_message_encode_buffer, message_size, 1u, 0, 0, 3);
        }
    }

    if ( !success )
    {
        object_type_delete(object_index);
        object_header_delete(object_header_data, object_index);
        object_index = -1;
out_of_objects:
        /* DEVIATION: callee is sprintf_0 per bl @0x836F1C84 (no plain sprintf symbol exists) */
        sprintf_0(message, "OUT OF OBJECTS: cannot create %s", tag_name_strip_path(tag_get_name(definition_index)));
        console_printf(0, "%s", message);
        return object_index;
    }

    int creation_effect = object_definition->creation_effect.index;
    if ( creation_effect == -1 )
        return object_index;

    effect_new_from_object(creation_effect, object_index, object_index, -1, 0.0, 0.0, 0, 0);
    return object_index;
}
