/* allow_client_side_weapon_projectiles_definition @ 0x8417EFD0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char allow_client_side_weapon_projectiles;

hs_global_external allow_client_side_weapon_projectiles_definition =
{
    "allow_client_side_weapon_projectiles",
    hs_type_boolean,
    { 0, 0 },
    &allow_client_side_weapon_projectiles,
    0
};
