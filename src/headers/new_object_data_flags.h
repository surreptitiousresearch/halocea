#ifndef NEW_OBJECT_DATA_FLAGS_H
#define NEW_OBJECT_DATA_FLAGS_H

/* new_object_data_flags — new_object_data flags.
 * DB-verified: types_enum_values _67982E78A58EC42DD5C90EAF9EE4D0E6 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum new_object_data_flags {
	_new_object_mirrored_bit = 0,
	_new_object_never_automatically_delete_bit = 1,
	NUMBER_OF_NEW_OBJECT_DATA_FLAGS = 2,
};

#endif /* NEW_OBJECT_DATA_FLAGS_H */

// new_object_data_flags: DB-verified via types_enum_values _67982E78A58EC42DD5C90EAF9EE4D0E6
