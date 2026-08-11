/* actor_look_secondary_stop @0x837FA7F8 — clear an actor's "secondary look" state (three 16-bit fields at
 * actor+0x544/0x546/0x548), used when a command list takes over and overrides looking behaviour. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/blam_data_globals.h"


void actor_look_secondary_stop(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->control.secondary_look_type     = _secondary_look_none;            /* word index 674 */
    actor->control.secondary_look_priority = _secondary_look_priority_none;   /* word index 675 */
    actor->control.secondary_look_timer    = 0;   /* word index 676 */
}
