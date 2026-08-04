#ifndef WIDGET_INTERNAL_FLAGS_H
#define WIDGET_INTERNAL_FLAGS_H

/* widget_internal_flags — widget_internal flags.
 * DB-verified: types_enum_values _47F7BA63F5B3A54672A7A4F871E02C14 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum widget_internal_flags {
	_widget_internal_zbuffer_bit = 0,
	_widget_internal_zbuffer_write_bit = 1,
	NUMBER_OF_WIDGET_INTERNAL_FLAGS = 2,
};

#endif /* WIDGET_INTERNAL_FLAGS_H */

// widget_internal_flags: DB-verified via types_enum_values _47F7BA63F5B3A54672A7A4F871E02C14
