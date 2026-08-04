/* weapon_place @0x836D84D0 — apply a scenario weapon placement's ammo/flag overrides to the newly placed
 * weapon object: if the weapon definition specifies a magazine (definition dword +1264 > 0, magazine data
 * at dword +1268), clamps the placement's rounds_total/rounds_loaded to the magazine's capacity (word +8)
 * and reserve limit (word +10) and stores them (weapon object word +694/+696). Sets/clears object flags
 * bit 0x20 at +16 (tracking placement flag 0x1) and +500 (tracking placement flag 0x4, inverted), always
 * ORs in +16 bit 0x20000, and — only when the "starts loaded" placement flag (0x1) is clear — nudges the
 * object's spawn-stagger float (+100) by 0.05. Returns weapon_index unchanged. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/scenario_weapon_datum.h"
#include "headers/object_flags.h"
#include "headers/scenario_weapon_flags.h"
#include "headers/item_flags.h"
#include "headers/blam_data_globals.h"


int weapon_place(int weapon_index, scenario_weapon_datum *scenario_weapon)
{
    weapon_datum *weapon =
        (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    if ( definition->weapon.magazines.count > 0 )
    {
        weapon_magazine_definition *magazine =
            (weapon_magazine_definition *)definition->weapon.magazines.address;

        __int16 rounds_total = scenario_weapon->rounds_total;
        if ( rounds_total > magazine->rounds_total_maximum )
            rounds_total = magazine->rounds_total_maximum;
        weapon->weapon.magazines[0].rounds_total = rounds_total;

        __int16 rounds_loaded = scenario_weapon->rounds_loaded;
        if ( rounds_loaded > magazine->rounds_loaded_maximum )
            rounds_loaded = magazine->rounds_loaded_maximum;
        weapon->weapon.magazines[0].rounds_loaded = rounds_loaded;
    }

    unsigned int flags16 = weapon->object.flags;
    flags16 |= (1u << _object_at_rest_bit);
    if ( (scenario_weapon->flags & (1u << _weapon_created_at_rest_bit)) == 0 )
        flags16 = weapon->object.flags & ~(1u << _object_at_rest_bit);
    weapon->object.flags = flags16;

    unsigned int flags500 = weapon->item.flags;
    weapon->object.flags = flags16 | (1u << _object_cannot_be_garbage_bit);

    unsigned int new_flags500 = flags500 | (1u << _item_does_not_accelerate_bit);
    if ( (scenario_weapon->flags & (1u << _weapon_does_accelerate_bit)) != 0 )
        new_flags500 = flags500 & ~(1u << _item_does_not_accelerate_bit);
    weapon->item.flags = new_flags500;

    if ( (scenario_weapon->flags & (1u << _weapon_created_at_rest_bit)) == 0 )
        weapon->object.position.n[2] = weapon->object.position.n[2] + 0.050000001f;

    return weapon_index;
}
