/* sound_cache_dump_to_file @ 0x837E7790 — debug dump of the sound cache to "sound_cache_dump.txt":
 * a header summarising page usage (allocated / used-this-frame / old / locked) and memory totals,
 * followed by one line per resident cached sound (tag name, compressed and uncompressed byte sizes).
 *
 * Deviation: the page-state classification and the MB / percent-free arithmetic are reconstructed
 * from the PPC float/int64-mangled decompiler output into their evident intent. The four page states
 * are read from the per-page usage bitmap (bit b set in page byte => that page is in state b):
 * state 0 = allocated, 1 = used this frame, 2 = old, 3 = locked. used_bytes = size * allocated/total. */

#include <stdint.h>
#include <string.h>
#include "headers/pc_sound_cache_globals.h"
#include "headers/data_iterator.h"
#include "headers/cache_sound_datum.h"
#include "headers/sound_permutation.h"
#include "headers/blam_data_globals.h"


extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern void lruv_cache_get_page_usage(lruv_cache *cache, unsigned char *page_usage);
extern char *tag_get_name(int tag_index);

#include <stdio.h> /* fopen/fwrite/fclose: use CRT decls to avoid signature conflict */
extern int sprintf_0(char *string, const char *format, ...);

void sound_cache_dump_to_file(void)
{
    unsigned int total_pages = sound_cache_page_count;
    unsigned char *page_usage = dlMalloc(
        sound_cache_page_count, "D:\\Projects\\code\\HCEX\\sources\\cache\\pc_sound_cache.c", 0x331u);
    struct _iobuf *file = fopen("sound_cache_dump.txt", "w+");
    char line[1120];

    int pages_allocated = 0;
    int pages_used_this_frame = 0;
    int pages_old = 0;
    int pages_locked = 0;
    int sounds_in_cache = 0;
    int line_number = 1;

    memset(line, 0, 0x400u);
    if ( file )
    {
        data_iterator iterator;
        int state;
        float free_fraction;
        float used_bytes;
        int length;
        cache_sound_datum *cache_sound;

        lruv_cache_get_page_usage(pc_sound_cache_globals.cache, page_usage);
        for ( state = 0; state < 4; ++state )
        {
            unsigned char mask = (unsigned char)(1 << state);
            unsigned int page;
            for ( page = 0; page < sound_cache_page_count; ++page )
            {
                if ( (page_usage[page] & mask) != 0 )
                {
                    if ( state == 0 )       ++pages_allocated;
                    else if ( state == 1 )  ++pages_used_this_frame;
                    else if ( state == 2 )  ++pages_old;
                    else                    ++pages_locked;
                }
            }
        }

        data_iterator_new(&iterator, pc_sound_cache_globals.cache_sounds);
        while ( data_iterator_next(&iterator) )
            ++sounds_in_cache;

        free_fraction = ((float)(total_pages - pages_allocated) / (float)total_pages);
        used_bytes = ((float)sound_cache_size * (float)pages_allocated / (float)total_pages);

        sprintf_0(line,
                  "%d / 512 sounds in cache\n"
                  "%.2f MB / %.2f MB used %.2f percent free\n"
                  "%d / %d pages allocated\n"
                  "%d / %d pages used this frame\n"
                  "%d / %d pages old\n"
                  "%d / %d pages locked\n"
                  "\n",
                  sounds_in_cache,
                  (double)(used_bytes / 1048576.0f), (double)((float)sound_cache_size / 1048576.0f),
                  (double)(free_fraction * 100.0f),
                  pages_allocated, total_pages,
                  pages_used_this_frame, total_pages,
                  pages_old, total_pages,
                  pages_locked, total_pages);

        for ( length = 0; line[length]; ++length )
            ;
        fwrite(line, 1u, length, file);

        memset(line, 0, 0x400u);
        fwrite("[sounds in cache]\n\n", 1u, 0x12u, file);

        data_iterator_new(&iterator, pc_sound_cache_globals.cache_sounds);
        for ( cache_sound = data_iterator_next(&iterator);
              cache_sound;
              cache_sound = data_iterator_next(&iterator) )
        {
            sound_permutation *loaded = cache_sound->sound;
            if ( loaded )
            {
                int uncompressed_bytes = loaded->unused0[0]; /* runtime: uncompressed sample byte size */
                int compressed_bytes = loaded->samples.size;
                const char *name = tag_get_name(loaded->runtime_tag_index);
                sprintf_0(line, "%d - %s %d c bytes %d u bytes\n",
                          line_number, name, compressed_bytes, uncompressed_bytes);
                for ( length = 0; line[length]; ++length )
                    ;
                fwrite(line, 1u, length, file);
                ++line_number;
            }
        }
        fclose(file);
    }
    dlFree(page_usage);
}
