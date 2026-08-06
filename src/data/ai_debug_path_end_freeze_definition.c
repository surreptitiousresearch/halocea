/* ai_debug_path_end_freeze_definition @ 0x84180930 (.data, 16 bytes)
 * DB applied_types: hs_global_external ai_debug_path_end_freeze_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211F4F8 -> "ai_debug_path_end_freeze"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x00000000
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

hs_global_external ai_debug_path_end_freeze_definition =
{
    "ai_debug_path_end_freeze",  /* name */
    hs_type_boolean,             /* type */
    { 0, 0 },                    /* _pad06.._pad07 */
    0,                           /* pointer */
    0,                           /* console_flags */
};
