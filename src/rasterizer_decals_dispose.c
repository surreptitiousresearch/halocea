/* rasterizer_decals_dispose @0x836A6094 — public decal-system shutdown guard: skip when running the
 * import tool (no live rasterizer), otherwise perform the real teardown.
 *
 * DEVIATION: the decompiler renders the call to _rasterizer_decals_dispose (leading underscore) as
 * self-recursion by dropping the underscore; the real callee is the distinct internal dispose. */

#include <stdint.h>

extern uint8_t shell_running_import_tool(void);
extern void _rasterizer_decals_dispose(void);

void rasterizer_decals_dispose(void)
{
    if ( !shell_running_import_tool() )
        _rasterizer_decals_dispose();
}
