/* weapon_must_be_readied @0x836D9390 — return the weapon definition's "must be readied" flag
 * (weapon-tag flags @0x308, bit 3): a weapon that cannot simply be auto-equipped without a ready
 * animation. Disasm (extrwi r3,r10,1,28) confirms bit 3 of _weapon_definition.flags. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/blam_data_globals.h"


/* Attested uint8_t return: all 5 sampled callers clrlwi r3,24 (byte-normalization). */
uint8_t weapon_must_be_readied(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    return (definition->weapon.flags & _weapon_definition_flag_must_be_readied) != 0;
}
