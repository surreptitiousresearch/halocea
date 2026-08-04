#ifndef DATA_FILE_S_H
#define DATA_FILE_S_H

#include <stdint.h>
#include "data_file_header_s.h"
#include "data_file_item_s.h"
#include "data_file_build_stats_s.h"

typedef struct data_file_s
{
    data_file_header_s     header;
    data_file_item_s      *items;
    uint32_t               max_item_count;
    uint32_t               max_names_len;
    uint32_t               cur_names_len;
    char                  *names;
    uint8_t                writable;
    uint8_t                _pad[3];
    data_file_build_stats_s hit_stats;
    data_file_build_stats_s miss_stats;
    const char            *file_name;
    void                  *hFile;
} data_file_s;

#endif /* DATA_FILE_S_H */
