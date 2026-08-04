#pragma once

/* 32-byte tag-instance table entry. */
typedef struct cache_file_tag_instance
{
    int group_tag;             // 0x00
    int parent_group_tags[2];  // 0x04
    int tag_index;             // 0x0C
    char *name;                // 0x10
    void *base_address;        // 0x14  pointer to the tag's data block
    unsigned int unused[2];    // 0x18
} cache_file_tag_instance;

extern cache_file_tag_instance *global_tag_instances;
