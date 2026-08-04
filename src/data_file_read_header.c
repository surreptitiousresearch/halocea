#include <stdio.h>
#include <stdint.h>
#include "headers/data_file_s.h"
#include "headers/blam_data_globals.h"

extern uint8_t cached_sync_read(void *file, void *buffer, int size, int offset);
extern void _byte_swap_memory(void *memory, int count, int code);
extern uint32_t GetLastError(void);


/* attest: uint8_t return — caller data_file_open normalizes with clrlwi r11,r3,24
   @ 0x83809830; exits are li r3,0 / li r3,1. */
uint8_t data_file_read_header(data_file_s *data_file, uint32_t data_file_id)
{
    uint32_t last_error;

    if (cached_sync_read(data_file->hFile, data_file, 16, 0))
    {
        if (data_file_reorder_on_write)
            _byte_swap_memory(data_file, 4, -4); /* li r5,-4 @ 0x838095AC */
        if (data_file->header.data_file_id == data_file_id)
        {
            return 1;
        }
        else
        {
            data_file->header.data_file_id  = 0;
            data_file->header.names_offset  = 0;
            data_file->header.items_offset  = 0;
            data_file->header.item_count    = 0;
            printf("Invalid data file id in data file %s\n", data_file->file_name);
            return 0;
        }
    }
    else
    {
        last_error = GetLastError();
        printf("Failed to read data file header %s %d\n", data_file->file_name, last_error);
        return 0;
    }
}
