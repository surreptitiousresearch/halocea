/* allow_client_side_weapon_projectiles_definition @ 0x8417EFD0 (.data, 16 bytes)
 * DB applied_types: hs_global_external allow_client_side_weapon_projectiles_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821220AC -> "allow_client_side_weapon_projectiles"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x841761F8 -> allow_client_side_weapon_projectiles
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char allow_client_side_weapon_projectiles;

hs_global_external allow_client_side_weapon_projectiles_definition =
{
    "allow_client_side_weapon_projectiles",  /* name */
    hs_type_boolean,                         /* type */
    { 0, 0 },                                /* _pad06.._pad07 */
    &allow_client_side_weapon_projectiles,   /* pointer */
    0,                                       /* console_flags */
};
