#ifndef CHILD_WIDGET_FLAGS_H
#define CHILD_WIDGET_FLAGS_H

/* child_widget_flags — child_widget flags.
 * DB-verified: types_enum_values _68FF42B98CA0C28551F0F0B71544C467 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum child_widget_flags {
	_child_widget_use_custom_controller_index_bit = 0,
	NUMBER_OF_CHILD_WIDGET_FLAGS = 1,
	MAXIMUM_NUMBER_OF_CHILD_WIDGET_REFERENCES = 32,
};

#endif /* CHILD_WIDGET_FLAGS_H */

// child_widget_flags: DB-verified via types_enum_values _68FF42B98CA0C28551F0F0B71544C467
