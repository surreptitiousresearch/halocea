#ifndef OBJECT_FLAGS_H
#define OBJECT_FLAGS_H

/* object_flags — bit indices for _object_datum.flags (unsigned int @0xC).
 * Source: reference enum _A9BA401F261A19C37F15A86CC3ABF8CA (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum object_flags {
	_object_invisible_bit = 0x0,
	_object_on_ground_bit = 0x1,
	_object_on_media_bit = 0x2,
	_object_partially_under_media_bit = 0x3,
	_object_wholly_under_media_bit = 0x4,
	_object_at_rest_bit = 0x5,
	_object_child_locations_invalid_bit = 0x6,
	_object_animates_automatically_bit = 0x7,
	_object_has_attached_lights_bit = 0x8,
	_object_has_attached_shader_bit = 0x9,
	_object_has_attached_looping_sounds_bit = 0xA,
	_object_connected_to_map_bit = 0xB,
	_object_mirrored_bit = 0xC,
	_object_dynamic_lighting_recompute_bit = 0xD,
	_object_static_lighting_recompute_bit = 0xE,
	_object_static_lighting_raycast_sideways_bit = 0xF,
	_object_garbage_bit = 0x10,
	_object_cannot_be_garbage_bit = 0x11,
	_object_shadowless_bit = 0x12,
	_object_deleted_when_deactivated_bit = 0x13,
	_object_cannot_be_activated_bit = 0x14,
	_object_outside_of_map_bit = 0x15,
	_object_movie_star_bit = 0x16,
	_object_do_not_recompute_node_matrices_bit = 0x17,
	_object_no_collisions_bit = 0x18,
	_object_has_collision_model_bit = 0x19,
	_object_force_baseline_update_bit = 0x1A,
	_object_has_been_updated_from_network_bit = 0x1B,
	NUMBER_OF_OBJECT_DATA_FLAGS = 0x1C,
};

#endif /* OBJECT_FLAGS_H */

// object_flags: DB-verified via types_enum_values _A9BA401F261A19C37F15A86CC3ABF8CA (anonymous compiled enum, names sans leading _)
