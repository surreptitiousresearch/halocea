/* cache_hardware_format_character @0x83768C38 — pack a font glyph into the hardware character cache atlas
 * bitmap, evicting older glyphs from the cache ring as needed. No-op if the glyph is already cached
 * (hardware_character_index != 0xFFFF). The atlas is a 512x512 bitmap filled left-to-right, top-to-bottom in
 * rows whose height grows to the tallest glyph; the cache slots form a 512-entry ring (read_index..write_index)
 * and a glyph is evicted (its font_character->hardware_character_index reset to -1) when the cursor wraps over
 * its row band or the ring fills. The glyph is blitted with a 1px white border; interior pixels take their
 * alpha from the font's 8bpp coverage source, and each destination texel is byte-swapped for the GPU format.
 *
 * Pure integer cache management; no float math, so no decompiler artifacts of note. */

#include <stdint.h>
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/hardware_character_cache.h"
#include "headers/byte_swap_definition.h"
#include "headers/blam_data_globals.h"


typedef struct bitmap_data bitmap_data;
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern void rasterizer_bitmap_changed(bitmap_data *bitmap);
extern void _byte_swap_memory(char *memory, int count, uint64_t code);
extern void flush_hardware_character(hardware_character *hardware_character);

void cache_hardware_format_character(font_header *font_header, font_character *font_character)
{
    if ((uint16_t)font_character->hardware_character_index != 0xFFFF)
        return;

    int bitmap_width = font_character->bitmap_width;
    font_character->pad = magic_number;

    /* wrap to the next atlas row if this glyph would overflow the 512px width */
    if (bitmap_width + hardware_character_cache.x0 + 2 > 512)
    {
        hardware_character_cache.x0 = 0;
        hardware_character_cache.y0 += hardware_character_cache.maximum_character_height;
        hardware_character_cache.maximum_character_height = 0;
    }

    int16_t write_index = hardware_character_cache.write_index;
    int16_t read_index = hardware_character_cache.read_index;

    /* wrap back to the top if it would overflow the bottom, evicting the still-pending tail */
    if (font_character->bitmap_height + hardware_character_cache.y0 + 2 >= 512)
    {
        int slot_index = hardware_character_cache.read_index;
        hardware_character_cache.y0 = 0;
        hardware_character_cache.x0 = 0;
        hardware_character_cache.maximum_character_height = 0;
        if (hardware_character_cache.read_index != hardware_character_cache.write_index)
        {
            do
            {
                hardware_character *slot = &hardware_character_cache.characters[slot_index];
                if (slot->y0 <= 0)
                    break;
                /* DEVIATION: collapsed inlined copy of flush_hardware_character@0x83768C18 (zero-xref) to its call; the two trailing write_index/read_index reloads were the inline's dup-datum-refetch artifact (provably no-ops here) and drop with the collapse. */
                flush_hardware_character(slot);
                read_index = (read_index + 1) & 0x1FF;
                slot_index = read_index;
                hardware_character_cache.read_index = read_index;
            } while (read_index != write_index);
        }
    }

    int bitmap_height = font_character->bitmap_height;
    /* grow the current row's height, evicting any glyphs that overlap the new row band */
    if (bitmap_height + 2 >= hardware_character_cache.maximum_character_height)
    {
        int slot_index = read_index;
        int16_t band_bottom = bitmap_height + hardware_character_cache.y0 + 2;
        if (read_index != write_index)
        {
            int band_top = (int16_t)(hardware_character_cache.maximum_character_height + hardware_character_cache.y0);
            do
            {
                hardware_character *slot = &hardware_character_cache.characters[slot_index];
                int slot_y0 = slot->y0;
                if (slot_y0 < band_top || slot_y0 >= band_bottom)
                    break;
                /* DEVIATION: collapsed inlined copy of flush_hardware_character@0x83768C18 (zero-xref) to its call; the two trailing read_index/write_index reloads were the inline's dup-datum-refetch artifact (provably no-ops here) and drop with the collapse. */
                flush_hardware_character(slot);
                read_index = (read_index + 1) & 0x1FF;
                slot_index = read_index;
                hardware_character_cache.read_index = read_index;
            } while (read_index != write_index);
        }
        hardware_character_cache.maximum_character_height = font_character->bitmap_height + 2;
    }

    /* if the ring is full, evict the oldest entry */
    if (((write_index + 1) & 0x1FF) == read_index)
    {
        /* DEVIATION: collapsed inlined copy of flush_hardware_character@0x83768C18 (zero-xref) to its call; donor arg reconstructed as &hardware_character_cache.characters[read_index]; the trailing write_index/read_index reloads were the inline's dup-datum-refetch artifact (provably no-ops here) and drop with the collapse. */
        flush_hardware_character(&hardware_character_cache.characters[read_index]);
        hardware_character_cache.read_index = (read_index + 1) & 0x1FF;
    }

    /* claim the write slot for this glyph */
    font_character->hardware_character_index = write_index;
    hardware_character *slot = &hardware_character_cache.characters[write_index];
    slot->character = font_character;
    slot->x0 = hardware_character_cache.x0;
    slot->y0 = hardware_character_cache.y0;

    /* blit the glyph into the atlas with a 1px white border; interior alpha comes from the 8bpp source */
    if (font_character->bitmap_height + 2 > 0)
    {
        char *source = (char *)font_header->pixels.address + font_character->pixels_offset - 1;
        int row = 0;
        do
        {
            int *dest = (int *)bitmap_2d_address(hardware_character_cache.bitmap, slot->x0, slot->y0 + row, 0);
            if (font_character->bitmap_width + 2 > 0)
            {
                int col = 0;
                do
                {
                    if (row < 1 || row > font_character->bitmap_height
                        || col < 1 || col > font_character->bitmap_width)
                        *dest = 0xFFFFFF;
                    else
                        *dest = ((uint8_t)*++source << 24) | 0xFFFFFF;
                    _byte_swap_memory((char *)dest, 1, _4byte);
                    ++dest;
                    col = (int16_t)(col + 1);
                } while (col < font_character->bitmap_width + 2);
            }
            row = (int16_t)(row + 1);
        } while (row < font_character->bitmap_height + 2);
    }

    /* inset the slot origin by the border, mark the atlas dirty, and advance the cache cursor */
    int16_t slot_y0 = slot->y0;
    ++slot->x0;
    slot->y0 = slot_y0 + 1;
    rasterizer_bitmap_changed(hardware_character_cache.bitmap);
    hardware_character_cache.x0 += font_character->bitmap_width + 2;
    hardware_character_cache.write_index = (hardware_character_cache.write_index + 1) & 0x1FF;
}
