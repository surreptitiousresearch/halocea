#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_idle_looking.h"
#include "headers/global_tag_instances.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"


actor_idle_looking * actor_look_get_looking_definition(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int variant_definition_index = actor->meta.definition_index;
    uint16_t looking_mode = actor->orders.look.idle_look_type;
    actor_definition *variant_definition = TAG_GET(actor_definition, variant_definition_index);

    if ( looking_mode == _idle_look_guarding )
        return &variant_definition->looking.idle_look_guard;          /* def+244 */
    if ( looking_mode <= _idle_look_guarding || looking_mode > _idle_look_combat )
        return &variant_definition->looking.idle_look_noncombat;      /* def+220 */
    return &variant_definition->looking.idle_look_search_combat;      /* def+268 */
}
