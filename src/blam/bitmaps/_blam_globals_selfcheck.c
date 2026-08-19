/* _blam_globals_selfcheck.c — compile-only self-check for the canonical globals header.
 *
 * Including src/headers/blam_data_globals.h in a single TU proves the header is internally
 * consistent: every one of the ~217 type headers it pulls resolves, and the 1429 extern
 * declarations do not conflict with each other or with the dedicated typed headers they defer
 * to. This is the compile-time half of the "start from the globals" guarantee; the source-level
 * half (no consumer diverges from canonical) is enforced by tools/check_global_externs.py.
 *
 * Regenerate the header with tools/gen_canonical_globals_header.py; it is AUTO-GENERATED from
 * the verified src/data/<sym>.c storage definitions (the single source of truth). */
#include "headers/blam_data_globals.h"

/* no code — this TU exists solely to type-check the canonical header */
