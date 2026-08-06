#pragma once
/* hud_messaging_globals — runtime HUD messaging state accessors. The struct bodies now live in their own
 * one-type-per-file headers (split pass); this header only pulls them together and exposes the globals.
 * hud_msg_def points at the active HUD messaging parameter definition (a sub-block of hud_globals). */

#include "hud_globals.h"
#include "hud_messaging_globals_definition.h"

#ifdef __cplusplus
extern "C" {
#endif

extern hud_messaging_globals_definition *hud_messaging_globals;
extern hud_messaging_parameters_definition *hud_msg_def;

#ifdef __cplusplus
}
#endif
