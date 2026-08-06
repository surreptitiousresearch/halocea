/* render_contrails_definition @ 0x8417FB60 (.data, 16 bytes)
 * DB applied_types: hs_global_external render_contrails_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82120C48 -> "render_contrails"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x00000000
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

hs_global_external render_contrails_definition =
{
    "render_contrails",  /* name */
    hs_type_boolean,     /* type */
    { 0, 0 },            /* _pad06.._pad07 */
    0,                   /* pointer */
    0,                   /* console_flags */
};
