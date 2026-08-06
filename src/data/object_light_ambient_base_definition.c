/* object_light_ambient_base_definition @ 0x8417F860 (.data, 16 bytes)
 * DB applied_types: hs_global_external object_light_ambient_base_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821211C4 -> "object_light_ambient_base"
 *   +0x04 type                       = 0x0006
 *   +0x08 pointer                    = 0x84176F00 -> object_light_ambient_base
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern float object_light_ambient_base;

hs_global_external object_light_ambient_base_definition =
{
    "object_light_ambient_base",  /* name */
    hs_type_real,                 /* type */
    { 0, 0 },                     /* _pad06.._pad07 */
    &object_light_ambient_base,   /* pointer */
    0,                            /* console_flags */
};
