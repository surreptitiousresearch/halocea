/* rasterizer_text_cache_dispose @0x83768B88 */
#include "headers/hardware_character_cache.h"

extern void rasterizer_text_cache_flush(void);
extern void bitmap_delete(bitmap_data *bitmap);

void rasterizer_text_cache_dispose(void)
{
    if (hardware_character_cache.initialized)
    {
        rasterizer_text_cache_flush();
        bitmap_delete(hardware_character_cache.bitmap);
        hardware_character_cache.initialized = 0;
    }
}
