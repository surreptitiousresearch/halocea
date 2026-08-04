#pragma once
/* cache_file_tags_header — header of the tag-instance table loaded from a .map cache file. */

#include "cache_file_tag_instance.h"

typedef struct cache_file_tags_header
{
    cache_file_tag_instance *tag_instances;            /* 0x00 */
    int                      scenario_tag_index;        /* 0x04 */
    unsigned int             tags_checksum;             /* 0x08 */
    int                      tag_count;                 /* 0x0C */
    int                      vertex_buffer_count;        /* 0x10 */
    void                    *vertex_buffers;             /* 0x14 */
    int                      index_buffer_count;         /* 0x18 */
    void                    *index_buffers;              /* 0x1C */
    int                      vertex_index_buffer_size;   /* 0x20 */
    unsigned int             signature;                  /* 0x24 */
} cache_file_tags_header;                                /* 40 bytes */
