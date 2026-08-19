/* sound_cache_debug_render @0x837E7BF0 — the PC sound-cache debug overlay (source: pc_sound_cache.c). Two
 * independent debug toggles:
 *   debug_sound_cache_graph: allocate a per-page usage-flag buffer, fill it from the LRU-V cache, and render the
 *     per-page usage graph. (In this release build the actual per-column draw compiled to nothing, leaving only
 *     the column/bit iteration skeleton; reproduced faithfully.)
 *   debug_sound_cache: allocate the same usage buffer, tally the pages set in each of the four usage states
 *     (allocated / used-this-frame / old / locked), count the cached sounds, compute used-vs-total megabytes, and
 *     format the stats string. (The draw_string call that would paint the formatted string likewise compiled out
 *     in release, so the string is built but not drawn here.)
 *
 * DEVIATION: the decompiler reported "local variable allocation has failed" and mis-rendered the sprintf argument
 * list (punned float/int registers). The 11 arguments were recovered from the disassembly (0x837E7DA8-837E7E48):
 * sound count; used MB (= size - (pages-allocated)*(size/pages)); total MB (= size); then four
 * "allocated/used/old/locked  /  page_count" integer pairs. */

#include <stdint.h>
#include "headers/pc_sound_cache_globals.h"
#include "headers/data_iterator.h"
#include <string.h>
#include "headers/blam_data_globals.h"


#include "headers/lruv_cache.h"
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern void lruv_cache_get_page_usage(lruv_cache *cache, unsigned char *page_usage);
extern void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count);
extern int sprintf_0(char *string, const char *format, ...);

void sound_cache_debug_render(void)
{
    if ( debug_sound_cache_graph )
    {
        uint8_t *page_usage = dlMalloc(sound_cache_page_count,
                "D:\\Projects\\code\\HCEX\\sources\\cache\\pc_sound_cache.c", 0x28C);
        lruv_cache_get_page_usage(pc_sound_cache_globals.cache, page_usage);

        /* release build: the per-column graph draw compiled to nothing, only the iteration remains */
        for ( int column = 640; column != 0; --column )
        {
            unsigned int bit_mask = 1;
            for ( int bit = 4; bit != 0; --bit )
                bit_mask = (bit_mask << 1) | (bit_mask >> 31);
        }

        dlFree(page_usage);
        return;
    }

    if ( !debug_sound_cache )
        return;

    int16_t tab_stops[3];
    tab_stops[0] = 280;
    tab_stops[1] = 0;
    tab_stops[2] = 0;

    unsigned int pages_allocated = 0;
    unsigned int pages_used_this_frame = 0;
    unsigned int pages_old = 0;
    unsigned int pages_locked = 0;

    uint8_t *page_usage = dlMalloc(sound_cache_page_count,
            "D:\\Projects\\code\\HCEX\\sources\\cache\\pc_sound_cache.c", 0x2C0);

    draw_string_set_tab_stops(tab_stops, 1);

    char string_buffer[4096];
    memset(string_buffer, 0, 0x1000);

    lruv_cache_get_page_usage(pc_sound_cache_globals.cache, page_usage);

    for ( int state = 0; state < 4; ++state )
    {
        unsigned int mask = 1 << state;
        for ( int page = 0; page < sound_cache_page_count; ++page )
        {
            if ( (page_usage[(int16_t)page] & mask) != 0 )
            {
                if ( state == 0 )
                    ++pages_allocated;
                else if ( state == 1 )
                    ++pages_used_this_frame;
                else if ( state == 2 )
                    ++pages_old;
                else
                    ++pages_locked;
            }
        }
    }

    data_iterator iterator;
    data_iterator_new(&iterator, pc_sound_cache_globals.cache_sounds);
    int cached_sound_count = 0;
    while ( data_iterator_next(&iterator) )
        ++cached_sound_count;

    float total_pages = (float)sound_cache_page_count;
    float allocated_pages_f = (float)pages_allocated;
    float size_mb = (float)sound_cache_size;
    double megabytes_total = (double)sound_cache_size;
    float megabytes_used = size_mb - (total_pages - allocated_pages_f) * (size_mb / total_pages);

    sprintf_0(string_buffer,
            "%d / 512 sounds in cache|n%.2f MB / %.2f MB used|n%d / %d pages allocated|n"
            "%d / %d pages used this frame|n%d / %d pages old|n%d / %d pages locked|n",
            cached_sound_count,
            megabytes_used, megabytes_total,
            pages_allocated, sound_cache_page_count,
            pages_used_this_frame, sound_cache_page_count,
            pages_old, sound_cache_page_count,
            pages_locked, sound_cache_page_count);

    dlFree(page_usage);
}
