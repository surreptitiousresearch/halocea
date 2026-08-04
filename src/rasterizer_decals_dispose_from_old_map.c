/* rasterizer_decals_dispose_from_old_map @0x836A605C — per-map decal teardown guard: skip when
 * running the import tool, otherwise perform the real per-map teardown.
 *
 * DEVIATION: the call to _rasterizer_decals_dispose_from_old_map (leading underscore) is rendered as
 * self-recursion by the decompiler; the real callee is the distinct internal dispose. */

#include <stdint.h>

extern uint8_t shell_running_import_tool(void);
extern void _rasterizer_decals_dispose_from_old_map(void);

void rasterizer_decals_dispose_from_old_map(void)
{
    if ( !shell_running_import_tool() )
        _rasterizer_decals_dispose_from_old_map();
}
