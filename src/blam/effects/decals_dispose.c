/* decals_dispose @ 0x83740568 — drop the decal pool and tear down rasterizer decals */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_decals_dispose(void);

/* attested void: tail call to void rasterizer_decals_dispose, sole caller (game_dispose) ignores r3 */
void decals_dispose(void)
{
    global_decal_data = 0;
    rasterizer_decals_dispose();
}
