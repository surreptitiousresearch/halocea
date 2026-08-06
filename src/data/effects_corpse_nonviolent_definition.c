/* effects_corpse_nonviolent_definition @ 0x8417F7C0 (.data, 16 bytes)
 * DB applied_types: hs_global_external effects_corpse_nonviolent_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821212B4 -> "effects_corpse_nonviolent"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x841764CC -> effects_corpse_nonviolent
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char effects_corpse_nonviolent;

hs_global_external effects_corpse_nonviolent_definition =
{
    "effects_corpse_nonviolent",  /* name */
    hs_type_boolean,              /* type */
    { 0, 0 },                     /* _pad06.._pad07 */
    &effects_corpse_nonviolent,   /* pointer */
    0,                            /* console_flags */
};
