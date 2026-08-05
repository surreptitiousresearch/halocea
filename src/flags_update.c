/* flags_update @0x8380F698 — per-frame cloth simulation step for every live flag object (5820-byte flag
 * datums): counts an update since the last render and runs flag_update for flags that are attached to an
 * object, were rendered recently (fewer than 5 updates since last render), and have a nonzero timestep. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/flag_datum.h"
#include "headers/flag_definition.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void flag_update(flag_datum *flag, flag_definition *flag_definition, float time);

void flags_update(float dt)
{
    for ( int flag_index = data_next_index(flag_data, -1); flag_index != -1;
          flag_index = data_next_index(flag_data, flag_index) )
    {
        flag_datum *flag = DATA_ARRAY_ELEMENT(flag_data, flag_datum, flag_index);
        flag_definition *definition = TAG_GET(flag_definition, flag->definition_index);
        int16_t updates_since_last_render = flag->updates_since_last_render + 1;
        flag->updates_since_last_render = updates_since_last_render;

        if ( flag->object_index != -1 && updates_since_last_render < 5 && dt != 0.0f )
            flag_update(flag, definition, dt);
    }
}
