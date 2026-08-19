/* projectile_handle_deleted_object @0x837597B0 */
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/blam_data_globals.h"


void projectile_handle_deleted_object(int projectile_index, int deleted_object_index)
{
    projectile_datum *projectile = (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    if ( projectile->projectile.target_object_index == deleted_object_index )
        projectile->projectile.target_object_index = -1;
}
