#ifndef DATA_FILE_HEADER_S_H
#define DATA_FILE_HEADER_S_H

#include <stdint.h>

typedef struct data_file_header_s
{
    uint32_t data_file_id;
    uint32_t names_offset;
    uint32_t items_offset;
    uint32_t item_count;
} data_file_header_s;

#endif /* DATA_FILE_HEADER_S_H */
