/* lightning_globals @ 0x844A7488 (.data, 4 bytes)
 * DB applied_types: $039D7A94EA641C1B0629B74F699EBAA2 lightning_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 lightning_data             = 0x00000000
 * DB anonymous struct $039D7A94EA641C1B0629B74F699EBAA2,
 * types.size = 4, one member: lightning_data:data_array * at +0x00. Kept as the one-member
 * struct the DB states (and that lightning_globals.h already declared) rather than collapsing
 * to a bare data_array *, so the member name survives — src/lightning_submit.c already externs
 * that typed view. Retyped from the size-equal unsigned char[4] blob. NOTE: DB size 4 vs ledger
 * gap 8 (DB authoritative).
 */
#include "../headers/lightning_globals.h"

lightning_globals_type lightning_globals;
