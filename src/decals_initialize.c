/* decals_initialize @ 0x83740470 — decal pool + decal globals + rasterizer decals */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern void *game_state_malloc(const char *name, const char *type, int size);
extern void rasterizer_decals_initialize(void);

void decals_initialize(void)
{
    global_decal_data = game_state_data_new("decals", 2048, 56);
    global_decal_data->identifier_zero_invalid = 1;
    decal_globals = game_state_malloc("decal globals", 0, 10252);
    rasterizer_decals_initialize(); /* attested void: r3-thread flattened */
}
