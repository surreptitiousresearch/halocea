/* game_engine_remap_object_definition @0x83748BD8 — when a multiplayer game engine is active, remap a
 * weapon/equipment object definition to the variant the current game variant prescribes (e.g. swap a
 * map's placed weapon for the gametype's loadout). Other object types and the no-engine case pass
 * through unchanged. The object type is the first word of the tag definition (1=object/unit base,
 * 2=weapon, 3=equipment). */

#include "headers/global_tag_instances.h"
#include "headers/object_type.h"

extern void *game_engine;
extern int game_engine_remap_weapon(int weapon_definition_index);
extern int game_engine_remap_equipment(int equipment_definition_index);

int game_engine_remap_object_definition(int definition_index)
{
    if ( game_engine && definition_index != -1 )
    {
        __int16 object_type = *TAG_GET(__int16, definition_index);
        if ( object_type == object_type_weapon )
            return game_engine_remap_weapon(definition_index);
        if ( object_type == object_type_equipment )
            return game_engine_remap_equipment(definition_index);
    }
    return definition_index;
}
