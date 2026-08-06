/* object_light_interpolate_definition @ 0x8417F890 (.data, 16 bytes)
 * DB applied_types: hs_global_external object_light_interpolate_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8212116C -> "object_light_interpolate"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x84176F0C -> object_light_interpolate
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char object_light_interpolate;

hs_global_external object_light_interpolate_definition =
{
    "object_light_interpolate",  /* name */
    hs_type_boolean,             /* type */
    { 0, 0 },                    /* _pad06.._pad07 */
    &object_light_interpolate,   /* pointer */
    0,                           /* console_flags */
};
