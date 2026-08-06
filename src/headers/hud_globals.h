#pragma once
/* hud_globals — HUD tag globals aggregate (hud.c).
 * Split into one-type-per-file: hud_messaging_parameters_definition and
 * hud_globals_definition now live in their own headers. */

#include "hud_messaging_parameters_definition.h"
#include "hud_globals_definition.h"

#ifdef __cplusplus
extern "C" {
#endif

extern hud_globals_definition *hud_globals;

#ifdef __cplusplus
}
#endif
