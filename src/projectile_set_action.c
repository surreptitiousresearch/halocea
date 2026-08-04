#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/blam_data_globals.h"


void projectile_set_action(int projectile_index, int16_t action)
{
    projectile_datum *projectile = (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;

    if (action > projectile->projectile.action)
        projectile->projectile.action = action;
}
