/* sound_cache_dump_to_file @ 0x837E7790 — debug dump of the sound cache to "sound_cache_dump.txt":
 * a header summarising page usage (allocated / used-this-frame / old / locked) and memory totals,
 * followed by one line per resident cached sound (tag name, compressed and uncompressed byte sizes).
 *
 * Deviation: the page-state classification is reconstructed from the PPC float/int64-mangled
 * decompiler output into its evident intent. The four page states are read from the per-page usage
 * bitmap (bit b set in page byte => that page is in state b): state 0 = allocated, 1 = used this
 * frame, 2 = old, 3 = locked.
 *
 * DEVIATION: the megabyte arithmetic previously divided by 1048576 and computed used = size *
 * allocated / total. Both were wrong. `sound_cache_size` is already a megabyte count (8), not a byte
 * count -- sound_cache_new derives the page count as size << 20 >> 12, i.e. MB -> 4 KB pages -- and
 * with the /1048576 the dump printed "0.00 MB / 0.00 MB". The binary prints the raw value as the
 * total and computes the used figure the same way sound_cache_debug_render does. Proof, all in
 * 0x837E78C4-0x837E7984: frsp f5 = (float)page_count @0x837E78F4, frsp f10 = (float)pages_allocated
 * @0x837E7934, frsp f7 = (float)sound_cache_size @0x837E792C; fsubs f3,f5,f10 @0x837E7944;
 * fdivs f4,f7,f5 @0x837E794C; fmuls f1,f3,f4 @0x837E7964 (free MB); fdivs f13,f1,f7 @0x837E7968
 * (free fraction); fsubs f1,f7,f1 @0x837E796C (used MB); fmuls f3,f13,f0 @0x837E7978 with
 * f0 = 100.0f (__real_42c80000 @0x837E7928). The total-MB argument is fcfid f2,f6 @0x837E793C with
 * no frsp -- a direct int->double of sound_cache_size, not a scaled float. */

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
        float total_pages_f;
        float allocated_pages_f;
        float megabytes_total;
        float megabytes_free;
        float free_fraction;
        float megabytes_used;
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

        total_pages_f = (float)total_pages;
        allocated_pages_f = (float)pages_allocated;
        megabytes_total = (float)sound_cache_size;
        megabytes_free = (total_pages_f - allocated_pages_f) * (megabytes_total / total_pages_f);
        free_fraction = megabytes_free / megabytes_total;
        megabytes_used = megabytes_total - megabytes_free;

        sprintf_0(line,
                  "%d / 512 sounds in cache\n"
                  "%.2f MB / %.2f MB used %.2f percent free\n"
                  "%d / %d pages allocated\n"
                  "%d / %d pages used this frame\n"
                  "%d / %d pages old\n"
                  "%d / %d pages locked\n"
                  "\n",
                  sounds_in_cache,
                  (double)megabytes_used, (double)sound_cache_size,
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
