/* rasterizer_decals_initialize_for_new_map @0x836A6020 — public wrapper: outside the import tool, rebuild the
 * ws-engine decal bridge from the current Blam decals. */

#include <stdint.h>

extern uint8_t shell_running_import_tool(void);
extern void _rasterizer_decals_initialize_for_new_map(void);

void rasterizer_decals_initialize_for_new_map(void)
{
    if ( !shell_running_import_tool() )
        _rasterizer_decals_initialize_for_new_map(); /* attested void: r3-thread residue, no real return */
}
