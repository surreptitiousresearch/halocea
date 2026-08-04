#ifndef FILE_SYSTEM_CHECK_RESULTS_H
#define FILE_SYSTEM_CHECK_RESULTS_H

/* file_system_check_results - values for widget_globals.filesystem_check_result.
 * DB-verified: types_enum_values _8629E1E74EB3720460F76B2525C705B7 (verbatim). */
enum file_system_check_results {
	_fscheck_result_no_error = 0,
	_fscheck_result_not_enough_free_space = 1,
	_fscheck_result_maximum_saved_games = 2,
	NUMBER_OF_FILE_SYSTEM_CHECK_RESULTS = 3,
};

#endif /* FILE_SYSTEM_CHECK_RESULTS_H */
