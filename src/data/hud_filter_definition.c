/* hud_filter_definition @ 0x84180C00 (.data, 16 bytes)
 * DB applied_types: hs_global_external hud_filter_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211F024 -> "hud_filter"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x844A5964 -> always_filter_hud
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char always_filter_hud;

hs_global_external hud_filter_definition =
{
    "hud_filter",        /* name */
    hs_type_boolean,     /* type */
    { 0, 0 },            /* _pad06.._pad07 */
    &always_filter_hud,  /* pointer */
    0,                   /* console_flags */
};
