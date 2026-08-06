/* transport_dumping_definition @ 0x8417EF70 (.data, 16 bytes)
 * DB applied_types: hs_global_external transport_dumping_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8212213C -> "transport_dumping"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x84185B24 -> transport_dumping
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char transport_dumping;

hs_global_external transport_dumping_definition =
{
    "transport_dumping",  /* name */
    hs_type_boolean,      /* type */
    { 0, 0 },             /* _pad06.._pad07 */
    &transport_dumping,   /* pointer */
    0,                    /* console_flags */
};
