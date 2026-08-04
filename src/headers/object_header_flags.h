#ifndef OBJECT_HEADER_FLAGS_H
#define OBJECT_HEADER_FLAGS_H

/* object_header_flags — bit indices for object_header_datum.flags (unsigned char @0x2).
 * DB-verified: types_enum_values _0DE4E1E08AF4A6D41C169D8E5AF0A83B carries these named
 * bit indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum object_header_flags {
	_object_header_active_bit = 0x0,
	_object_header_visible_bit = 0x1,
	_object_header_being_created_bit = 0x2,
	_object_header_being_deleted_bit = 0x3,
	_object_header_do_not_update_bit = 0x4,
	_object_header_connected_to_map_bit = 0x5,
	_object_header_automatically_deactivate_bit = 0x6,
	_object_header_child_bit = 0x7,
	NUMBER_OF_OBJECT_HEADER_FLAGS = 0x8,
};

#endif /* OBJECT_HEADER_FLAGS_H */

// object_header_flags: DB-verified via types_enum_values _0DE4E1E08AF4A6D41C169D8E5AF0A83B (anonymous compiled enum, names sans leading _)
