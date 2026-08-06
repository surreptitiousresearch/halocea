/* net_bandwidth_definition @ 0x8417EF90 (.data, 16 bytes)
 * DB applied_types: hs_global_external net_bandwidth_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82122114 -> "net_bandwidth"
 *   +0x04 type                       = 0x0008
 *   +0x08 pointer                    = 0x844ABA98 -> gNetMeterBandwidth
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int gNetMeterBandwidth;

hs_global_external net_bandwidth_definition =
{
    "net_bandwidth",       /* name */
    hs_type_long_integer,  /* type */
    { 0, 0 },              /* _pad06.._pad07 */
    &gNetMeterBandwidth,   /* pointer */
    0,                     /* console_flags */
};
