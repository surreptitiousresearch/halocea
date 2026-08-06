/* sv_mapcycle_timeout_definition @ 0x84180CB0 (.data, 16 bytes)
 * DB applied_types: hs_global_external sv_mapcycle_timeout_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211EF28 -> "sv_mapcycle_timeout"
 *   +0x04 type                       = 0x0008
 *   +0x08 pointer                    = 0x844613C0 -> gMapCycleTimeoutSeconds
 *   +0x0C console_flags              = 0x5F
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int gMapCycleTimeoutSeconds;

hs_global_external sv_mapcycle_timeout_definition =
{
    "sv_mapcycle_timeout",     /* name */
    hs_type_long_integer,      /* type */
    { 0, 0 },                  /* _pad06.._pad07 */
    &gMapCycleTimeoutSeconds,  /* pointer */
    95,                        /* console_flags */
};
