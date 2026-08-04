#ifndef DATA_FILE_ITEM_S_H
#define DATA_FILE_ITEM_S_H

#include <stdint.h>

typedef struct data_file_item_s
{
    uint32_t name_offset;
    uint32_t data_size;
    uint32_t data_offset;
} data_file_item_s;

#endif /* DATA_FILE_ITEM_S_H */
