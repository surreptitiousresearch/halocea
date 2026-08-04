/* scenario_structure_bsp_unload @0x836F74DC — unload the active structure BSP: quiesce render jobs and
 * defrag, drop the BSP tag's loaded data pointer, and clear the cached BSP header. */

#include "headers/scenario_structure_bsp_reference.h"
#include "headers/cache_file_tag_instance.h"
#include "headers/cache_file_tags_globals.h"
#include "headers/blam_data_globals.h"

extern void hcex_wait_jobs_and_defrag(void);

void scenario_structure_bsp_unload(const scenario_structure_bsp_reference *reference)
{
    hcex_wait_jobs_and_defrag();
    global_tag_instances[(short)reference->structure_bsp.index].base_address = 0;
    cache_file_globals.structure_bsp_header = 0;
}
