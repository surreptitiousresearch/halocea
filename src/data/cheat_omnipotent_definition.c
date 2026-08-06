/* cheat_omnipotent_definition @ 0x8417F7A0 (.data, 16 bytes)
 * DB applied_types: hs_global_external cheat_omnipotent_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821212E4 -> "cheat_omnipotent"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8477BE27 -> "" (empty string at target)
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/cheats.h"
extern cheat_globals cheat;

hs_global_external cheat_omnipotent_definition =
{
    "cheat_omnipotent",  /* name */
    hs_type_boolean,     /* type */
    { 0, 0 },            /* _pad06.._pad07 */
    (char *)&cheat + 7,  /* pointer */
    0,                   /* console_flags */
};
