/* scenario_tags_load @0x836F72F0 — open the named .map cache file, verify its header, stream the tag-instance
 * table into the tag cache base address, publish the loaded tag globals, and return the scenario tag index
 * (or -1 on failure to open/verify). */

#include <stdint.h>
#include "headers/cache_file_tags_globals.h"
#include "headers/cache_file_read_request_params.h"
#include "headers/cache_file_tag_instance.h"

#include "headers/cache_file_header.h"
#include "headers/cache_file_header.h"
extern const char *tag_name_strip_path(const char *name);
extern void texture_cache_open(void);
extern void sound_cache_open(void);
extern uint8_t cache_file_open(const char *scenario_name, cache_file_header *header);
extern void *physical_memory_get_tag_cache_base_address(void);
extern uint8_t cache_file_header_verify(const cache_file_header *header, const char *name, uint8_t fatal);
extern int16_t cache_file_read(int tag_index, uint32_t offset, int size, void *buffer, const cache_file_read_request_params *params, uint8_t blocking, uint8_t data_file);
extern int   SwitchToThread(void);
extern void tags_header_save_offsets(void);
extern void tag_register_vbufs(cache_file_tags_header *tags_header);

int scenario_tags_load(const char *name)
{
    const char *stripped = tag_name_strip_path(name);
    /* deviation: decompiler chains these as sound_cache_open(texture_cache_open()) via leftover r3; both are
     * void no-arg calls — they are independent. */
    texture_cache_open();
    sound_cache_open();

    if ( !cache_file_open(stripped, &cache_file_globals.header) )
        return -1;

    cache_file_tags_header *tag_cache_base_address =
        physical_memory_get_tag_cache_base_address();
    if ( !cache_file_header_verify(&cache_file_globals.header, name, 1u) )
        return -1;

    volatile unsigned char finished = 0;
    cache_file_read_request_params params;
    params.finished_func = 0;
    params.finished_flag = &finished;
    params.userdata = 0;

    int read_result = cache_file_read(-1,
                                      cache_file_globals.header.tags_offset,
                                      cache_file_globals.header.tags_size,
                                      tag_cache_base_address,
                                      &params, 1u, 0);
    while ( !finished )
        read_result = SwitchToThread();

    cache_file_globals.tags_header = tag_cache_base_address;
    global_tag_instances = tag_cache_base_address->tag_instances;
    cache_file_globals.tags_loaded = 1;
    tags_header_save_offsets();

    int scenario_tag_index = cache_file_globals.tags_header->scenario_tag_index;
    tag_register_vbufs(cache_file_globals.tags_header);
    return scenario_tag_index;
}
