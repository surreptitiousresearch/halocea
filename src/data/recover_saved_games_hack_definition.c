/* recover_saved_games_hack_definition @ 0x8417F650 (.data, 16 bytes)
 * DB applied_types: hs_global_external recover_saved_games_hack_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821214BC -> "recover_saved_games_hack"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8477C3BC -> recover_saved_games_hack
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char recover_saved_games_hack;

hs_global_external recover_saved_games_hack_definition =
{
    "recover_saved_games_hack",  /* name */
    hs_type_boolean,             /* type */
    { 0, 0 },                    /* _pad06.._pad07 */
    &recover_saved_games_hack,   /* pointer */
    0,                           /* console_flags */
};
