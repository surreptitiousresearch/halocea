#ifndef THREAD_ATTRIBUTE_FLAGS_H
#define THREAD_ATTRIBUTE_FLAGS_H

/* thread_attribute_flags - bit indices for the create_thread flags word.
 * DB-verified: types_enum_values $D017F4674DABBCA33AECC6DADEA65F87 carries these names verbatim.
 * Values are BIT INDICES - use as (1u << _bit): create_thread tests (flags & 2) for
 * priority_low (SetThreadPriority(-1)) and (flags & 4) for priority_high (+1), matching
 * bits 1 and 2 respectively. */
enum thread_attribute_flags {
	_thread_attribute_flag_default = 0,
	_thread_attribute_flag_priority_low = 1,
	_thread_attribute_flag_priority_high = 2,
	NUMBER_OF_THREAD_ATTRIBUTE_FLAGS = 3,
};

#endif /* THREAD_ATTRIBUTE_FLAGS_H */
