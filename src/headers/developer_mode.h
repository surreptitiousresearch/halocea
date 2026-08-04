#ifndef DEVELOPER_MODE_H
#define DEVELOPER_MODE_H

/* developer_mode - error_globals.developer_mode threshold levels.
 * DB-verified: types_enum_values $A2F90FFD7372F69773E8DD66E217ADF9 carries these verbatim. */
enum developer_mode {
	_developer_mode_off = 0,
	_developer_mode_errors_enabled = 1,
	_developer_mode_logging_enabled = 2,
	_developer_mode_metrics_enabled = 3,
	_developer_mode_conprint_enabled = 4,
	_developer_mode_full = 127,
};

#endif /* DEVELOPER_MODE_H */
