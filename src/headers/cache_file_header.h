#ifndef CACHE_FILE_HEADER_H
#define CACHE_FILE_HEADER_H

#include <stdint.h>

typedef struct cache_file_header
{
    int32_t  header_signature;
    int32_t  version;
    int32_t  size;
    int32_t  compressed_file_padding;
    int32_t  tags_offset;
    int32_t  tags_size;
    int32_t  index_buffer_count;
    int32_t  index_buffers_offset;
    char     name[32];
    char     build_number[32];
    int16_t  scenario_type;
    uint16_t pad;
    uint32_t checksum;
    uint32_t unused2[485];
    int32_t  footer_signature;
} cache_file_header;

#endif /* CACHE_FILE_HEADER_H */
