#pragma once
/* cheats — runtime cheat-toggle flags (one byte each). Global instance is `cheat`. */
// DB-verified via types_members cheat_globals (reconstructed source spelled it `cheats`).

/* The struct body lives in its DB-named home; this header only adds the Blam source spelling.
 * (Was a second file-scope definition behind a BLAM_CHEAT_GLOBALS_STRUCT guard — identical
 * member-for-member, but a guard is not a single definition: include order decided which body a
 * TU saw, and the header-layout probe measured the type twice.) */
#include "cheat_globals.h"

typedef cheat_globals cheats;  /* Blam source spelling; DB type is cheat_globals */

#ifdef __cplusplus
extern "C" {
#endif

extern cheats cheat;

#ifdef __cplusplus
}
#endif
