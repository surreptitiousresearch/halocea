#include "headers/blam_data_globals.h"

int _field_type_item_placement_position_size_calculator(void *field_properties_definition)
{
    return gITEM_PLACEMENT_BITS_Z + gITEM_PLACEMENT_BITS_Y + gITEM_PLACEMENT_BITS_X;
}
