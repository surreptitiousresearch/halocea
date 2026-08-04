#include <stdint.h>
#include "headers/blam_data_globals.h"
/* material_get_name @0x837B66C8 — display name for a global material type, or "NONE" for -1. */


const char * material_get_name(int16_t material_type)
{
    if (material_type == -1)
        return "NONE";
    return global_material_type_strings[material_type];
}
