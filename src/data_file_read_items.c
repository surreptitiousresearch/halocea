/* data_file_read_items @0x83809698 */
#include <stdio.h>
#include <stdint.h>
#include "headers/data_file_s.h"
#include "headers/data_file_item_s.h"
#include "headers/byte_swap_definition.h"
#include "headers/blam_data_globals.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern uint8_t cached_sync_read(void *file, void *buffer, int size, int offset);
/* extern fixed: _byte_swap_memory's `code` arg is a 32-bit int (`li r5,-4`), not uint64_t. */
extern void _byte_swap_memory(void *memory, int count, int code);


uint8_t data_file_read_items(data_file_s *data_file)
{
    uint32_t          items_size;
    data_file_item_s *items;
    int               read_ok;

    items_size       = sizeof(data_file_item_s) * data_file->header.item_count;
    items            = dlMalloc(items_size,
                           "D:\\Projects\\code\\HCEX\\sources\\cache\\data_file.c",
                           0xCEu);
    data_file->items = items;
    read_ok          = cached_sync_read(data_file->hFile, items, items_size,
                                        data_file->header.items_offset);

    if (data_file_reorder_on_write)
        _byte_swap_memory(data_file->items,
                         (3 * data_file->header.item_count) & 0x3FFFFFFF,
                         _4byte);

    if (read_ok)
        data_file->max_item_count = data_file->header.item_count;
    else
        printf("Invalid format in data file %s\n", data_file->file_name);

    return read_ok;
}
