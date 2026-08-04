/* action_alert_control @0x838266D0 — enters the actor's "alert" state: sets the actor's idle look to
 * noncombat, and if the actor definition is flagged crouch-when-noncombat (actor_definition.flags bit
 * _actor_definition_crouch_when_noncombat_bit == 0x40), also latches the stationary_crouch/moving_crouch bytes.
 * DEVIATION: meta.definition_index resolves to the actor_definition tag (not the unit tag); bit 0x40 named
 * from actor_definition_flags (was mis-annotated as a "unit always-eyes-open" flag). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"


void action_alert_control(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    actor->orders.look.idle_look_type = _idle_look_noncombat;

    if ((actor_def->flags & (1u << _actor_definition_crouch_when_noncombat_bit)) != 0)
    {
        actor->orders.move.stationary_crouch = 1;
        actor->orders.move.moving_crouch = 1;
    }
}
