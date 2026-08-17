/* biped_disconnect_from_structure_bsp @0x837AC7E8 */
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/blam_data_globals.h"


void biped_disconnect_from_structure_bsp(int biped_index)
{
    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    biped->biped.support_surface_index = -1;
    biped->biped.pathfinding_surface_index = -1;
    biped->biped.last_pathfinding_surface_index = -1;
}
