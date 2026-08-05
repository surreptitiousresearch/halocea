/* rasterizer_text_cache_flush @0x83768B40 — invalidate the hardware glyph cache: detach every cached font
 * character (marking its hardware slot index invalid) and clear the cache slots. */

#include <stdint.h>
#include "headers/hardware_character_cache.h"
#include "headers/font_character.h"

extern void flush_hardware_character(hardware_character *hardware_character);
void rasterizer_text_cache_flush(void)
{
    if ( hardware_character_cache.initialized )
    {
        for ( int16_t i = 0; i < 512; i = (int16_t)(i + 1) )
        {
            /* DEVIATION: inlined flush_hardware_character@0x83768C18 (zero-xref donor) inside the 512-slot flush loop; compiler hoisted the always-safe `.character = 0` store outside the guard (idempotent when already null), collapsed back to a per-iteration call */
            flush_hardware_character(&hardware_character_cache.characters[i]);
        }
    }
}
