/* breakable_surfaces_definition @ 0x841800C0 (.data, 16 bytes)
 * DB applied_types: hs_global_external breakable_surfaces_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82120220 -> "breakable_surfaces"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8417D409 -> breakable_surface_effect_enabled
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char breakable_surface_effect_enabled;

hs_global_external breakable_surfaces_definition =
{
    "breakable_surfaces",               /* name */
    hs_type_boolean,                    /* type */
    { 0, 0 },                           /* _pad06.._pad07 */
    &breakable_surface_effect_enabled,  /* pointer */
    0,                                  /* console_flags */
};
