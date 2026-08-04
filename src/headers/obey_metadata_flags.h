#ifndef OBEY_METADATA_FLAGS_H
#define OBEY_METADATA_FLAGS_H

/* obey_metadata_flags — bit indices for the obey_metadata flags word.
 * DB-verified: types_enum_values _064D83530768B30696F7F5D8DE361DE7 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum obey_metadata_flags {
	_obey_metadata_targeting_bit = 0,
	_obey_metadata_commands_finished_bit = 1,
	_obey_metadata_action_changed_bit = 2,
	_obey_metadata_told_to_advance_bit = 3,
	_obey_metadata_waiting_for_advance_notification_bit = 4,
	NUMBER_OF_OBEY_METADATA_FLAGS = 5,
};

#endif /* OBEY_METADATA_FLAGS_H */

// obey_metadata_flags: DB-verified via types_enum_values _064D83530768B30696F7F5D8DE361DE7
