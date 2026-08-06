/* decals_definition @ 0x841800D0 (.data, 16 bytes)
 * DB applied_types: hs_global_external decals_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211D5CC -> "decals"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8417D408 -> decals_enabled
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char decals_enabled;

hs_global_external decals_definition =
{
    "decals",         /* name */
    hs_type_boolean,  /* type */
    { 0, 0 },         /* _pad06.._pad07 */
    &decals_enabled,  /* pointer */
    0,                /* console_flags */
};
