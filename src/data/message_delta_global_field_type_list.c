/* message_delta_global_field_type_list @ 0x84182E10, 672 bytes = 28 x _field_type_definition
   (24 bytes each). Dispatch table for the network message-delta field serializer: one row
   per _field_type enum id, each carrying requires_parameters, a maximum-size calculator, a
   parameter verifier, and a destructor. Reconstructed from the raw image (big-endian); all
   84 function pointers resolved to named functions via the DB funcs table. The trailing
   `initialized` byte of every row is 0 in the image (populated at runtime). */
#include <stdint.h>
#include "../headers/field_type_definition.h"

/* forward decls for the field-type callback functions (defined elsewhere in the Blam lib) */
typedef struct _field_properties_definition _field_properties_definition;
extern int _field_type_integer_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_integer_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_integer_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_real_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_real_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_real_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_boolean_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_boolean_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_boolean_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_ascii_character_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_ascii_character_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_ascii_character_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_wide_character_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_wide_character_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_wide_character_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_ascii_string_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_ascii_string_parameter_verifier(const _field_properties_definition *);
extern void _field_type_ascii_string_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_wide_string_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_wide_string_parameter_verifier(const _field_properties_definition *);
extern void _field_type_wide_string_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_arbitrary_data_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_arbitrary_data_parameter_verifier(const _field_properties_definition *);
extern void _field_type_arbitrary_data_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_array_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_array_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_array_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_structure_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_structure_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_structure_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_pointer_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_pointer_parameter_verifier(const _field_properties_definition *);
extern void _field_type_pointer_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_enumeration_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_enumeration_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_enumeration_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_bounded_index_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_bounded_index_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_bounded_index_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_translated_index_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_translated_index_parameter_verifier(const _field_properties_definition *);
extern void _field_type_translated_index_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_point_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_point_parameter_verifier(const _field_properties_definition *);
extern void _field_type_point_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_vector_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_vector_parameter_verifier(const _field_properties_definition *);
extern void _field_type_vector_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_angle_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_angle_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_angle_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_flags_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_flags_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_flags_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_time_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_time_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_time_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_grenade_counts_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_grenade_counts_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_grenade_counts_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_fixed_width_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_fixed_width_parameter_verifier(const _field_properties_definition *);
extern void _field_type_fixed_width_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_fixed_width_normal_vector_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_fixed_width_normal_vector_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_fixed_width_normal_vector_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_locality_reference_position_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_locality_reference_position_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_locality_reference_position_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_digital_throttle_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_digital_throttle_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_digital_throttle_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_fixed_width_weapon_index_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_fixed_width_weapon_index_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_fixed_width_weapon_index_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_fixed_width_grenade_index_size_calculator(_field_properties_definition *const field_properties_definition);
extern uint8_t _field_type_fixed_width_grenade_index_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_fixed_width_grenade_index_destructor(const _field_properties_definition *const field_properties_definition);
extern int             _field_type_smart_vector_size_calculator(_field_properties_definition *);
extern uint8_t _field_type_smart_vector_parameter_verifier(const _field_properties_definition *);
extern void _field_type_smart_vector_destructor(const _field_properties_definition *const field_properties_definition);
extern int _field_type_item_placement_position_size_calculator(void *field_properties_definition);
extern uint8_t _field_type_item_placement_position_parameter_verifier(const _field_properties_definition *const field_properties_definition);
extern void _field_type_item_placement_position_destructor(const _field_properties_definition *const field_properties_definition);

_field_type_definition message_delta_global_field_type_list[28] = {
    { _field_type_integer, 1, {0,0,0}, _field_type_integer_size_calculator, _field_type_integer_parameter_verifier, _field_type_integer_destructor },
    { _field_type_real, 0, {0,0,0}, _field_type_real_size_calculator, _field_type_real_parameter_verifier, _field_type_real_destructor },
    { _field_type_boolean, 0, {0,0,0}, _field_type_boolean_size_calculator, _field_type_boolean_parameter_verifier, _field_type_boolean_destructor },
    { _field_type_ascii_character, 0, {0,0,0}, _field_type_ascii_character_size_calculator, _field_type_ascii_character_parameter_verifier, _field_type_ascii_character_destructor },
    { _field_type_wide_character, 0, {0,0,0}, _field_type_wide_character_size_calculator, _field_type_wide_character_parameter_verifier, _field_type_wide_character_destructor },
    { _field_type_ascii_string, 1, {0,0,0}, _field_type_ascii_string_size_calculator, _field_type_ascii_string_parameter_verifier, _field_type_ascii_string_destructor },
    { _field_type_wide_string, 1, {0,0,0}, _field_type_wide_string_size_calculator, _field_type_wide_string_parameter_verifier, _field_type_wide_string_destructor },
    { _field_type_arbitrary_data, 1, {0,0,0}, _field_type_arbitrary_data_size_calculator, _field_type_arbitrary_data_parameter_verifier, _field_type_arbitrary_data_destructor },
    { _field_type_array, 1, {0,0,0}, _field_type_array_size_calculator, _field_type_array_parameter_verifier, _field_type_array_destructor },
    { _field_type_structure, 1, {0,0,0}, _field_type_structure_size_calculator, _field_type_structure_parameter_verifier, _field_type_structure_destructor },
    { _field_type_pointer, 1, {0,0,0}, _field_type_pointer_size_calculator, _field_type_pointer_parameter_verifier, _field_type_pointer_destructor },
    { _field_type_enumeration, 1, {0,0,0}, _field_type_enumeration_size_calculator, _field_type_enumeration_parameter_verifier, _field_type_enumeration_destructor },
    { _field_type_bounded_index, 1, {0,0,0}, _field_type_bounded_index_size_calculator, _field_type_bounded_index_parameter_verifier, _field_type_bounded_index_destructor },
    { _field_type_translated_index, 1, {0,0,0}, _field_type_translated_index_size_calculator, _field_type_translated_index_parameter_verifier, _field_type_translated_index_destructor },
    { _field_type_point, 1, {0,0,0}, _field_type_point_size_calculator, _field_type_point_parameter_verifier, _field_type_point_destructor },
    { _field_type_vector, 1, {0,0,0}, _field_type_vector_size_calculator, _field_type_vector_parameter_verifier, _field_type_vector_destructor },
    { _field_type_angle, 0, {0,0,0}, _field_type_angle_size_calculator, _field_type_angle_parameter_verifier, _field_type_angle_destructor },
    { _field_type_flags, 1, {0,0,0}, _field_type_flags_size_calculator, _field_type_flags_parameter_verifier, _field_type_flags_destructor },
    { _field_type_time, 0, {0,0,0}, _field_type_time_size_calculator, _field_type_time_parameter_verifier, _field_type_time_destructor },
    { _field_type_grenade_counts, 0, {0,0,0}, _field_type_grenade_counts_size_calculator, _field_type_grenade_counts_parameter_verifier, _field_type_grenade_counts_destructor },
    { _field_type_fixed_width, 1, {0,0,0}, _field_type_fixed_width_size_calculator, _field_type_fixed_width_parameter_verifier, _field_type_fixed_width_destructor },
    { _field_type_fixed_width_normal_vector, 1, {0,0,0}, _field_type_fixed_width_normal_vector_size_calculator, _field_type_fixed_width_normal_vector_parameter_verifier, _field_type_fixed_width_normal_vector_destructor },
    { _field_type_locality_reference_position, 0, {0,0,0}, _field_type_locality_reference_position_size_calculator, _field_type_locality_reference_position_parameter_verifier, _field_type_locality_reference_position_destructor },
    { _field_type_digital_throttle, 0, {0,0,0}, _field_type_digital_throttle_size_calculator, _field_type_digital_throttle_parameter_verifier, _field_type_digital_throttle_destructor },
    { _field_type_fixed_width_weapon_index, 0, {0,0,0}, _field_type_fixed_width_weapon_index_size_calculator, _field_type_fixed_width_weapon_index_parameter_verifier, _field_type_fixed_width_weapon_index_destructor },
    { _field_type_fixed_width_grenade_index, 0, {0,0,0}, _field_type_fixed_width_grenade_index_size_calculator, _field_type_fixed_width_grenade_index_parameter_verifier, _field_type_fixed_width_grenade_index_destructor },
    { _field_type_smart_vector, 1, {0,0,0}, _field_type_smart_vector_size_calculator, _field_type_smart_vector_parameter_verifier, _field_type_smart_vector_destructor },
    { _field_type_item_placement_position, 0, {0,0,0}, _field_type_item_placement_position_size_calculator, _field_type_item_placement_position_parameter_verifier, _field_type_item_placement_position_destructor },
};
