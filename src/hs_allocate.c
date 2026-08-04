/* hs_allocate @ 0x8372EC50 — allocate the HaloScript syntax-node pool and bind it to
 * the current scenario (or mark it standalone-allocated if there is no scenario). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_data.h"
#include "headers/tag_block.h"
extern data_array *data_new(const char *name, int16_t maximum_count, int16_t size);
extern void data_make_valid(data_array *data);
extern void dlFree(void *ptr);
extern uint8_t tag_data_resize(tag_data *data, int size);
extern uint8_t tag_block_resize(tag_block *block, int element_count);


void hs_allocate(void)
{
    scenario *scn;
    data_array *nodes;

    scn = (global_scenario_index == -1) ? 0 : global_scenario;
    if ( scn && scn->hs_syntax_data.size == 380076 )
        return;  /* already bound */

    nodes = data_new("script node", 19001, 20);
    hs_syntax_data = nodes;
    if ( !nodes )
        return;

    data_make_valid(nodes);
    if ( scn )
    {
        dlFree(scn->hs_syntax_data.address);
        scn->hs_syntax_data.size = 380076;
        scn->hs_syntax_data.address = hs_syntax_data;
        tag_data_resize(&scn->hs_string_constants, 1024);
        tag_block_resize(&scn->hs_scripts, 0);
    }
    else
    {
        hs_syntax_data_allocated = 1;
    }
}
