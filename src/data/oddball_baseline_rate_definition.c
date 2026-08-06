/* oddball_baseline_rate_definition @ 0x8417F010 (.data, 16 bytes)
 * DB applied_types: hs_global_external oddball_baseline_rate_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82122044 -> "oddball_baseline_rate"
 *   +0x04 type                       = 0x0008
 *   +0x08 pointer                    = 0x84185930 -> oddball_baseline_rate
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int oddball_baseline_rate;

hs_global_external oddball_baseline_rate_definition =
{
    "oddball_baseline_rate",  /* name */
    hs_type_long_integer,     /* type */
    { 0, 0 },                 /* _pad06.._pad07 */
    &oddball_baseline_rate,   /* pointer */
    0,                        /* console_flags */
};
