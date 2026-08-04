#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/squad_datum.h"
#include "headers/blam_data_globals.h"


squad_datum * encounter_get_squad(encounter_datum *encounter, int16_t squad_index)
{
    return &squad_array[(short)(encounter->squad_base + squad_index)];
}
