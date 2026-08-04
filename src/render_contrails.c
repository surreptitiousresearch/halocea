/* render_contrails @0x837EB1C8 — render every active contrail whose definition render-type matches the
 * requested render_type_flags bitmask. Each contrail datum tracks up to 4 point lists; a list is drawn only
 * when it has at least 2 points. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/contrail_datum.h"
#include "headers/contrail_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void *datum_get(data_array *data, int index);
extern void render_contrail(contrail_datum *contrail, contrail_definition *contrail_definition, int16_t instance_index);

void render_contrails(unsigned int render_type_flags)
{
    for (int i = data_next_index(contrail_data, -1); i != -1; i = data_next_index(contrail_data, i))
    {
        contrail_datum *contrail =
            datum_get(contrail_data, i);
        contrail_definition *definition =
            TAG_GET(contrail_definition, contrail->definition_index);

        __int16 point_list = 0;
        do
        {
            if (((1 << definition->render_type) & render_type_flags) != 0
                && contrail->contrail_point_counts[point_list] >= 2)
                render_contrail(contrail, definition, point_list);
            point_list = (__int16)(point_list + 1);
        } while (point_list < 4);
    }
}
