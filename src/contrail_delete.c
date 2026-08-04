/* contrail_delete @0x8373F060 — free a contrail and all of its points. Each of the 4 point lists is a
 * singly-linked chain through contrail_point_data (next_contrail_point_index); every point is freed, then
 * the contrail datum itself. */

#include "headers/data_array.h"
#include "headers/contrail_datum.h"
#include "headers/contrail_point_datum.h"
#include "headers/blam_data_globals.h"


extern void datum_delete(data_array *data, int index);

void contrail_delete(int contrail_index)
{
    contrail_datum *contrail = DATA_ARRAY_ELEMENT(contrail_data, contrail_datum, contrail_index);
    int list;

    for ( list = 0; list < 4; list = (__int16)(list + 1) )
    {
        int point = contrail->first_contrail_point_indices[list];
        while ( point != -1 )
        {
            int next = DATA_ARRAY_ELEMENT(contrail_point_data, contrail_point_datum, point)->next_contrail_point_index;
            datum_delete(contrail_point_data, point);
            point = next;
        }
    }
    datum_delete(contrail_data, contrail_index);
}
