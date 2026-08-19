/* scenario_structure_bsp_load @0x836F73E8 — synchronously loads a structure BSP block from the cache
 * file into its reserved address, spinning (yielding + servicing sound) until the read completes, then
 * publishes the loaded BSP header, points the tag instance at the parsed BSP data, and registers its
 * vertex buffers with the rasterizer. Always returns 1. */

#include <stdint.h>
#include "headers/scenario_structure_bsp_reference.h"
#include "headers/cache_file_read_request_params.h"
#include "headers/cache_file_tags_globals.h"
#include "headers/cache_file_structure_bsp_header.h"
#include "headers/cache_file_tag_instance.h"
#include "headers/blam_data_globals.h"


extern int16_t cache_file_read(int tag_index, uint32_t offset, int size, void *buffer, const cache_file_read_request_params *params, uint8_t blocking, uint8_t data_file);
extern int SwitchToThread(void);
extern uint32_t system_milliseconds(void);
extern int sound_render_time(void);
extern void sound_idle(void);
extern void bsp_register_vbufs(cache_file_structure_bsp_header *structure_bsp_header);
extern void hcex_after_register_vertex_buffers(void);

uint8_t scenario_structure_bsp_load(const scenario_structure_bsp_reference *reference)
{
    int size = reference->size;
    int offset = reference->offset;

    volatile uint8_t finished = 0;
    cache_file_read_request_params params;
    params.finished_func = nullptr;
    params.finished_flag = &finished;
    params.userdata = nullptr;
    cache_file_read(-1, offset, size, reference->address, &params, 1u, 0);

    while (!finished)
    {
        SwitchToThread();
        if (system_milliseconds() - sound_render_time() > 0x21)
            sound_idle();
    }

    cache_file_globals.structure_bsp_header = (cache_file_structure_bsp_header *)reference->address;
    global_tag_instances[(int16_t)reference->structure_bsp.index].base_address =
        cache_file_globals.structure_bsp_header->structure_bsp;
    bsp_register_vbufs(cache_file_globals.structure_bsp_header);
    hcex_after_register_vertex_buffers();
    return 1;
}
