/* hs_scenario_postprocess @ 0x8372FF30 — (re)compile/post-process the scenario's
 * HaloScript: bind the syntax-node pool, then either accept existing compiled data or
 * recompile from source, resizing the string-constant block as needed.
 * `hack` forces a recompile and restores the previous syntax pool on exit. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_block.h"
#include "headers/tag_data.h"
#include "headers/tag_groups.h"
extern void hs_allocate(void);
extern void data_delete_all(data_array *data);
extern uint8_t hs_compile_postprocess(const char **error_message_pointer, const char **error_source_pointer);
extern uint8_t hs_compile_source(void);

uint8_t hs_scenario_postprocess(uint8_t hack)
{
    int ok = 1;
    data_array *saved_syntax = hs_syntax_data;
    scenario *scn = global_scenario;
    const char *err;
    const char *err_table[19];
    unsigned char force_compile;

    hs_allocate();

    /* recompile if there are no scripts yet but there are source files */
    force_compile = (!scn->hs_scripts.count && scn->hs_source_files.count > 0);
    if ( hack )
        force_compile = 1;

    hs_syntax_data = (data_array *)scn->hs_syntax_data.address;
    hs_syntax_data->data = &hs_syntax_data[1];

    if ( !force_compile && hs_compile_postprocess(err_table, &err) )
    {
        int size = scn->hs_string_constants.size;
        if ( size < 1024 )
            ok = tag_data_resize(&scn->hs_string_constants, size + 1024);
    }
    else if ( hs_compile_source() && hs_compile_postprocess(err_table, &err) )
    {
        ok = 1;
    }
    else
    {
        data_delete_all(hs_syntax_data);
        if ( tag_block_resize(&scn->hs_globals, 0) && tag_block_resize(&scn->hs_scripts, 0) )
            tag_data_resize(&global_scenario->hs_string_constants, 1024);
        ok = 0;
    }

    if ( hack )
        hs_syntax_data = saved_syntax;
    return ok;
}
