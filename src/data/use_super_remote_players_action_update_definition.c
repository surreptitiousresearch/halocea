/* use_super_remote_players_action_update_definition @ 0x8417F950 (.data, 16 bytes)
 * DB applied_types: hs_global_external use_super_remote_players_action_update_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82120FE0 -> "use_super_remote_players_action_update"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8417E224 -> use_super_remote_players_action_update
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char use_super_remote_players_action_update;

hs_global_external use_super_remote_players_action_update_definition =
{
    "use_super_remote_players_action_update",  /* name */
    hs_type_boolean,                           /* type */
    { 0, 0 },                                  /* _pad06.._pad07 */
    &use_super_remote_players_action_update,   /* pointer */
    0,                                         /* console_flags */
};
