#ifndef TAG_GROUP_FLAGS_H
#define TAG_GROUP_FLAGS_H

/* tag_group_flags — tag_group flags.
 * DB-verified: types_enum_values _4E702CEF752DE25624C01B860A84B15C carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum tag_group_flags {
	_tag_group_is_game_critical_bit = 0,
	_tag_group_is_local_bit = 1,
	_tag_group_is_not_editable_bit = 2,
	_tag_group_can_be_reloaded_bit = 3,
	NUMBER_OF_TAG_GROUP_FLAGS = 4,
};

#endif /* TAG_GROUP_FLAGS_H */

// tag_group_flags: DB-verified via types_enum_values _4E702CEF752DE25624C01B860A84B15C
