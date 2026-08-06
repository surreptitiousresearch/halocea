/* sv_public_definition @ 0x84180CC0 (.data, 16 bytes)
 * DB applied_types: hs_global_external sv_public_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211EF1C -> "sv_public"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8417ED84 -> sv_public_value
 *   +0x0C console_flags              = 0x15
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char sv_public_value;

hs_global_external sv_public_definition =
{
    "sv_public",       /* name */
    hs_type_boolean,   /* type */
    { 0, 0 },          /* _pad06.._pad07 */
    &sv_public_value,  /* pointer */
    21,                /* console_flags */
};
