/* hcex_check_ach34 @0x83683EE0 — HCEX achievement 34 check ("d20" map, Heroic or above): iterate every
 * unit object counting live elites (model tag path contains "characters\elite\elite", object flags word
 * +262 bit 2 clear = not dead); if none remain, fire the ACH34 player event. Also fired via goto when the
 * very first iterator call returns no objects at all. */

#include <stdint.h>
#include "../headers/object_iterator.h"
#include "../headers/object_damage_flags.h"
#include "../headers/global_tag_instances.h"
#include "../headers/object_type.h"
#include "../headers/object_datum.h"
#include "../headers/_object_definition.h"
#include "../headers/game_difficulty.h"

extern char *main_get_map_name(void);
extern int16_t game_difficulty_level_get(void);
extern char *strstr(const char *string, const char *substring);
extern void hcex_fire_plr_event(const char *event_name, short player_identifier);

extern "C" void hcex_check_ach34(void)
{
    const char *map_name = main_get_map_name();
    int living_elite_count = 0;

    if ( strstr(map_name, "d20") && game_difficulty_level_get() >= game_difficulty_level_hard )
    {
        object_iterator iterator;
        object_iterator_new(&iterator, object_mask_biped, 0);
        object_datum *object = (object_datum *)object_iterator_next(&iterator);
        if ( !object )
            goto fire_event;
        do
        {
            const char *model_tag_path =
                (*(_object_definition **)TAG_INSTANCE(object->definition_index))->model.name;   /* objdef+44 */
            if ( model_tag_path && strstr(model_tag_path, "characters\\elite\\elite")
              && (object->object.damage_flags & (1u << _object_dead_bit)) == 0 )   /* object+262 */
                ++living_elite_count;
            object = (object_datum *)object_iterator_next(&iterator);
        }
        while ( object );
        if ( !living_elite_count )
fire_event:
            hcex_fire_plr_event("ACH34", -1);
    }
}
