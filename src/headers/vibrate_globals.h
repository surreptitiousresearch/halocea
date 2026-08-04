#pragma once
/* vibrate_globals — DB-verified: applied_types @0x8445FF00 is
 * `vibrate_global_data *vibrate_globals;`. The prior standalone reconstruction
 * `vibrate_globals_s` was an incomplete duplicate (missing scripted_vibrate) and
 * clashed with vibrate_global_data.h. Alias the reconstruction name to the DB
 * type so the data def (src/data/vibrate_globals.c) and blam_data_globals.h
 * both resolve to the canonical `vibrate_global_data *`. */

#include "vibrate_global_data.h"

typedef vibrate_global_data vibrate_globals_s;

extern vibrate_global_data *vibrate_globals;
