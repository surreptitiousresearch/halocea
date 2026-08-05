/* contrails_reconnect_to_structure_bsp @0x8373F100 — after a structure-BSP switch, recompute the BSP
 * location of every live contrail point (56-byte records: location @20, position @28, next-index dword[13]
 * @52) in each contrail's 4 point lists (list heads at contrail+52), skipping points whose cluster index
 * (location+4) is NONE. Counterpart of the no-op contrails_disconnect_from_structure_bsp. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/contrail_datum.h"
#include "headers/contrail_point_datum.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void scenario_location_from_point(location *location, const real_point3d *point);

void contrails_reconnect_to_structure_bsp(void)
{
    for ( int contrail_index = data_next_index(contrail_data, -1); contrail_index != -1;
          contrail_index = data_next_index(contrail_data, contrail_index) )
    {
        contrail_datum *contrail = DATA_ARRAY_ELEMENT(contrail_data, contrail_datum, contrail_index);
        for ( int16_t list_index = 0; list_index < 4; list_index++ )
        {
            for ( int point_index = contrail->first_contrail_point_indices[list_index]; point_index != -1; )
            {
                contrail_point_datum *point = DATA_ARRAY_ELEMENT(contrail_point_data, contrail_point_datum, point_index);
                if ( (uint16_t)point->location.cluster_index != 0xFFFF )
                    scenario_location_from_point(&point->location, &point->position);
                point_index = point->next_contrail_point_index;
            }
        }
    }
}
