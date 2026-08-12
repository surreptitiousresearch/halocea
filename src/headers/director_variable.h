#pragma once

/* director_variable — which of the four debug "director variables" (the free/debug camera's
 * smoothed height / roll / forward / right axes) an index selects. It is the index domain of both
 * director_variable_definition variables[4] (src/data/variables.c) and director.debug_variables[4]
 * (director.h), which director_process_variables walks together. director_update_controls drives
 * controls->position_delta.n[2] — the up axis — from debug_variables[0], which pins
 * _variable_height = 0. Source: types_enum_values $183BF03658F44CDE05C978EF42912EF2 (DB-verified).
 * Member names are the DB's own, verbatim; the enum tag is spelled for the DB count member
 * (NUMBER_OF_DIRECTOR_VARIABLES) because the bare member prefix "variable" is not a usable tag. */
enum director_variable {
	_variable_height = 0,
	_variable_roll = 1,
	_variable_forward = 2,
	_variable_right = 3,
	NUMBER_OF_DIRECTOR_VARIABLES = 4,
};

// director_variable: DB-verified via types_enum_values $183BF03658F44CDE05C978EF42912EF2
