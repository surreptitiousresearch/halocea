/* display_vblank_deltas_definition @ 0x8417EF40 (.data, 16 bytes)
 * DB applied_types: hs_global_external display_vblank_deltas_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8212217C -> "display_vblank_deltas"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8441E429 -> display_vblank_deltas
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char display_vblank_deltas;

hs_global_external display_vblank_deltas_definition =
{
    "display_vblank_deltas",  /* name */
    hs_type_boolean,          /* type */
    { 0, 0 },                 /* _pad06.._pad07 */
    &display_vblank_deltas,   /* pointer */
    0,                        /* console_flags */
};
