#pragma once
/* input_gamepad — compatibility aliases onto the DB-authoritative input layouts.
 * The canonical types are gamepad_state (gamepad_state.h) and input_globals_xbox
 * (input_globals_xbox.h / input_globals.h); the previously-modeled local bodies
 * (fabricated _pre[16] prefix, local axis2d) were superseded by the DB-verified
 * layouts. */

#include "gamepad_state.h"
#include "input_globals.h"

typedef point2d axis2d;                  /* == DB point2d { __int16 n[2] } */
typedef gamepad_state input_gamepad;     /* 40 bytes */
typedef input_globals_xbox input_globals_t;
