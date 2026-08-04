/* rasterizer_decals_initialize @0x836A5FC0 (ledger alias: rasterizer_decals_initialize_hw) — the
 * hardware-path decal initializer. When not running as the import tool (a real game/render session),
 * delegate to the shared decal-buffer allocator _rasterizer_decals_initialize (@0x83710D60). Under the
 * import tool it does nothing. No value is computed: r3 at return is only callee residue and the one
 * caller (decals_initialize) ignores it, so the attested return is void. */

#include <stdint.h>
#include "headers/lruv_cache.h"

extern uint8_t shell_running_import_tool(void);
/* _rasterizer_decals_initialize @0x83710D60 — allocates the D3D decal vertex buffer + LRUV cache
 * (returns lruv_cache*; only its truthiness is used here). */
extern void _rasterizer_decals_initialize(void);

void rasterizer_decals_initialize(void)
{
    if ( !shell_running_import_tool() )
        _rasterizer_decals_initialize();
}
