/* light_volume_globals @ 0x844A7540 (.data, 4 bytes)
 * DB applied_types: $0C063A19751579468C6CF468ED4D2770 light_volume_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 light_volume_data          = 0x00000000
 * DB anonymous struct $0C063A19751579468C6CF468ED4D2770,
 * types.size = 4, one member: light_volume_data:data_array * at +0x00. Kept as the one-member
 * struct the DB states (and that light_volume_globals.h already declared) rather than collapsing
 * to a bare data_array *, so the member name survives. Retyped from the size-equal
 * unsigned char[4] blob. NOTE: DB size 4 vs ledger gap 8 (DB authoritative).
 */
#include "../headers/light_volume_globals.h"

light_volume_globals_type light_volume_globals;
