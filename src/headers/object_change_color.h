#pragma once

/* object_change_color — which of an object's four change-color (tint) slots an index selects. It is
 * the index domain of object.base_change_colors[4] / object.outgoing_change_colors[4] (_object.h)
 * and of an object definition's change_colors tag block (object_change_color_definition):
 * object_choose_random_change_colors fills all four 0..3, and attachments_new derives the slot it
 * hands light_new / effect_new_looping as (attachment_ref->change_color_reference - 1), which maps
 * _change_color_reference_a onto _object_change_color_a.
 * Source: types_enum_values $240C990AE7594E11BF71060494B20239 (DB-verified). */
enum object_change_color {
	_object_change_color_a = 0,
	_object_change_color_b = 1,
	_object_change_color_c = 2,
	_object_change_color_d = 3,
	NUMBER_OF_OBJECT_CHANGE_COLORS = 4,
};

// object_change_color: DB-verified via types_enum_values $240C990AE7594E11BF71060494B20239
