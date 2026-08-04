#include <stdint.h>
#include "headers/cinematic_globals.h"

extern uint8_t cinematic_in_progress(void);
extern void object_types_place_objects(uint8_t place);

void object_types_reconnect_to_structure_bsp(void)
{
    if (!cinematic_in_progress() || !cinematic_globals->cinematic_suppress_bsp_object_creation)
        object_types_place_objects(1u);
}
