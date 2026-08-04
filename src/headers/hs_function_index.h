#ifndef HS_FUNCTION_INDEX_H
#define HS_FUNCTION_INDEX_H

/* hs_function_index — slot order of the built-in special-form entries of hs_function_table
 * (0x84178048; verified against the table's name strings: [22]="inspect", [23]="unit", ...).
 * Source: DB anonymous enum _A88F73F8A1FD257C6F0FD8B752183112 (types_enum_values, verbatim). */
enum hs_function_index {
	_hs_function_begin = 0,
	_hs_function_begin_random = 1,
	_hs_function_if = 2,
	_hs_function_cond = 3,
	_hs_function_set = 4,
	_hs_function_and = 5,
	_hs_function_or = 6,
	_hs_function_plus = 7,
	_hs_function_minus = 8,
	_hs_function_times = 9,
	_hs_function_divide = 10,
	_hs_function_min = 11,
	_hs_function_max = 12,
	_hs_function_equal = 13,
	_hs_function_not_equal = 14,
	_hs_function_gt = 15,
	_hs_function_lt = 16,
	_hs_function_gte = 17,
	_hs_function_lte = 18,
	_hs_function_sleep = 19,
	_hs_function_sleep_until = 20,
	_hs_function_wake = 21,
	_hs_function_inspect = 22,
	_hs_function_object_to_unit = 23,
	_hs_function_debug_string__first = 24,
	_hs_function_debug_string__ai_debug_communication_suppress = 24,
	_hs_function_debug_string__ai_debug_communication_ignore = 25,
	_hs_function_debug_string__ai_debug_communication_focus = 26,
	_hs_function_debug_string__last = 26,
};

#endif /* HS_FUNCTION_INDEX_H */
