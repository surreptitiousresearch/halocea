#pragma once
/* input_globals @ input_globals — raw per-frame input device state (input.c, Xbox backend). */

#include "input_globals_xbox.h"

/* input_point2d was an orphan duplicate of DB point2d; use point2d (point2d.h,
 * included via input_globals_xbox.h -> gamepad_state.h) instead. */
typedef point2d input_point2d;

extern input_globals_xbox input_globals;
