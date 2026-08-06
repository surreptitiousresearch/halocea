/* default_damage_material @ 0x84429E78 (.data, 72 bytes)
 * DB applied_types: damage_material default_damage_material;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x0000000000000000000000000000000000000000000000000000000000000000
 *   +0x20 flags                      = 0x00000000
 *   +0x24 type                       = 0x0000
 *   +0x26 pad                        = 0x0000
 *   +0x28 shield_leak_fraction       = 0x00000000 -> 0f
 *   +0x2C shield_damage_multiplier   = 0x00000000 -> 0f
 *   +0x30 shield_unused              = 0x000000000000000000000000
 *   +0x3C body_damage_multiplier     = 0x00000000 -> 0f
 *   +0x40 body_unused                = 0x0000000000000000
 * data-bss zero-fill; DB applied-type `damage_material default_damage_material;`.
 */
#include "../headers/damage_material.h"

damage_material default_damage_material;
