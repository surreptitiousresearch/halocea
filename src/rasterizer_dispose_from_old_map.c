#include "headers/blam_data_globals.h"
/* rasterizer_dispose_from_old_map @ 0x83724188 — flush screen effects + text cache */
extern void rasterizer_screen_effects_dispose_from_old_map(void);
extern void rasterizer_text_cache_flush(void);
void rasterizer_dispose_from_old_map(void) /* attested void: 0/1 callers consume r3 */
{
    rasterizer_screen_effects_dispose_from_old_map();
    rasterizer_text_cache_flush(); /* attested void(void): r3-thread removed */
    global_rasterizer_data = 0;
}
