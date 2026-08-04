/* lruv_debug_to_file @0x8371CA90 — dump the state of an LRU/validation cache to a text file for debugging a
 * failed allocation. Writes a caller-supplied header (via header_proc), a summary line (page/block counts, the
 * failed allocation's name/size/page-count), then one line per page describing the block that occupies it: the
 * block's page span, its age in frames (clamped to 9999), an "L" marker when the block is locked or was touched
 * this frame, and the block's name (via name_block_proc). Runs of pages with no block are collapsed into a
 * single blank-named line.
 *
 * DEVIATION: the "(#%d pages)" summary argument is ceil(failed_allocation_size / page_size); the decompiler lost
 * the store that computes it (rendered as an uninitialized local) — recovered from disasm. */

#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

typedef struct _iobuf _iobuf;

extern _iobuf *fopen(const char *filename, const char *mode);
extern int fprintf(_iobuf *stream, const char *format, ...);
extern int fclose(_iobuf *stream);
extern char empty_string[];   /* shared empty-string global */

void lruv_debug_to_file(const char *path, const char *failed_allocation_name, int failed_allocation_size,
        lruv_cache *cache, void (*header_proc)(_iobuf *), const char *(*name_block_proc)(int))
{
    _iobuf *file = fopen(path, "w+");
    if ( !file )
        return;

    fprintf(file, "%s (v1: only blocks used this frame are locked)\n", cache->name);
    header_proc(file);

    int page_size = 1 << cache->page_size_bits;
    int failed_allocation_pages = (failed_allocation_size >> cache->page_size_bits)
                                + ((failed_allocation_size & (page_size - 1)) != 0 ? 1 : 0);
    fprintf(file,
            "\n"
            "#%d pages, each #%d bytes\n"
            "#%d blocks at frame index #%d\n"
            "failed allocation of \"%s\" was #%d bytes (#%d pages)\n"
            "\n",
            cache->page_count, page_size, cache->blocks->actual_count, cache->frame_index,
            failed_allocation_name, failed_allocation_size, failed_allocation_pages);

    int page = 0;
    int block_index = cache->first_block_index;
    while ( page < cache->page_count )
    {
        unsigned int age = 0;
        char locked = 0;
        int span;
        const char *name;

        if ( block_index == -1 )
        {
            span = cache->page_count - page;
            page = cache->page_count;
            name = empty_string;
        }
        else
        {
            lruv_cache_block *block = &((lruv_cache_block *)cache->blocks->data)[(unsigned __int16)block_index];
            if ( page == block->first_page_index )
            {
                span = block->page_count;
                age = cache->frame_index - block->last_used_frame_index;
                if ( cache->locked_block_proc && cache->locked_block_proc(block_index) )
                    locked = 1;
                if ( block->last_used_frame_index + 1 >= cache->frame_index )
                    locked = 1;
                page = block->first_page_index + block->page_count;
                name = name_block_proc(block_index);
                block_index = block->next_block_index;
                if ( !name )
                    name = empty_string;
            }
            else
            {
                span = block->first_page_index - page;
                page = block->first_page_index;
                name = empty_string;
            }
        }

        int display_age = age >= 0x270F ? 9999 : (int)age;
        const char *lock_marker = locked ? "L" : " ";
        fprintf(file, "%s % 5d% 5d %s\n", lock_marker, span, display_age, name);
    }

    fprintf(file, "\n");
    fclose(file);
}
