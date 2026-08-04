#pragma once
/* _field_type — network-serialized field type id, indexes message_delta_global_field_type_list[28].
 * DB-verified (types_enum_values _field_type, 29 values). */

typedef enum _field_type
{
    _field_type_integer = 0,
    _field_type_real = 1,
    _field_type_boolean = 2,
    _field_type_ascii_character = 3,
    _field_type_wide_character = 4,
    _field_type_ascii_string = 5,
    _field_type_wide_string = 6,
    _field_type_arbitrary_data = 7,
    _field_type_array = 8,
    _field_type_structure = 9,
    _field_type_pointer = 10,
    _field_type_enumeration = 11,
    _field_type_bounded_index = 12,
    _field_type_translated_index = 13,
    _field_type_point = 14,
    _field_type_vector = 15,
    _field_type_angle = 16,
    _field_type_flags = 17,
    _field_type_time = 18,
    _field_type_grenade_counts = 19,
    _field_type_fixed_width = 20,
    _field_type_fixed_width_normal_vector = 21,
    _field_type_locality_reference_position = 22,
    _field_type_digital_throttle = 23,
    _field_type_fixed_width_weapon_index = 24,
    _field_type_fixed_width_grenade_index = 25,
    _field_type_smart_vector = 26,
    _field_type_item_placement_position = 27,
    _field_type_LAST_POSITION = 28,
} _field_type;
