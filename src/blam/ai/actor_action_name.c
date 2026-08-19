/* actor_action_name @0x837F3DC8 */
#include <stdint.h>
#include "headers/action_specification.h"
#include "headers/actor_action.h"

const char * actor_action_name(int16_t actor_action_index)
{
    if ( actor_action_index >= 0 && actor_action_index < number_of_actor_actions )
        return global_action_functions[actor_action_index].name;
    return "unknown";
}
