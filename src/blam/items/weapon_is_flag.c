/* weapon_is_flag @0x836D93D8 — true if the weapon object's definition tests bit 3 of the weapon
 * definition flags. Disasm (extrwi r3,r10,1,28) is byte-identical to weapon_must_be_readied: both read
 * _weapon_definition.flags @0x308 bit 3 (_weapon_definition_flag_must_be_readied). Despite the "is_flag"
 * name, the DB proves it is the same bit, so the named constant is used verbatim. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/blam_data_globals.h"


/* Attested uint8_t return: all 12 sampled callers clrlwi r3,24 (byte-normalization). */
uint8_t weapon_is_flag(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    return (definition->weapon.flags & _weapon_definition_flag_must_be_readied) != 0;
}
