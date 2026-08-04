#ifndef OBJECT_TYPE_H
#define OBJECT_TYPE_H

/* object_type — the object's class (_object_datum.type, and object-definition type fields).
 * Source: reference enum _F5AF3705676D16CDF71575CA63411CB1 (DB-verified, types_enum_values). The
 * object-mask bitfield values share the same anonymous enum in the DB and are reproduced here in order. */
enum object_type {
	object_type_biped = 0,
	object_type_vehicle = 1,
	object_type_weapon = 2,
	object_type_equipment = 3,
	object_type_garbage = 4,
	object_type_projectile = 5,
	object_type_scenery = 6,
	object_type_machine = 7,
	object_type_control = 8,
	object_type_light_fixture = 9,
	object_type_placeholder = 10,
	object_type_sound_scenery = 11,
	number_of_object_types = 12,
	object_mask_all = -1,
	object_mask_unit = 3,
	object_mask_biped = 1,
	object_mask_vehicle = 2,
	object_mask_item = 28,
	object_mask_weapon = 4,
	object_mask_equipment = 8,
	object_mask_garbage = 16,
	object_mask_projectile = 32,
	object_mask_scenery = 64,
	object_mask_sound_scenery = 2048,
	object_mask_device = 896,
	object_mask_machine = 128,
	object_mask_control = 256,
	object_mask_light_fixture = 512,
	object_mask_placeholder = 1024,
	object_mask_sightblocking = 194,
	object_mask_cannot_interpolate = 4064,
	object_mask_remove_on_bsp_switch = 576,
};

#endif /* OBJECT_TYPE_H */

// object_type: DB-verified via types_enum_values _F5AF3705676D16CDF71575CA63411CB1 (anonymous compiled enum, names sans leading _)
