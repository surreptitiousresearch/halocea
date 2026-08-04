/* hs_compile_dispose @0x8377A228 — finish/clean up after a compile pass. For a permanent (full-recompile)
 * pass: on a compile error, roll back the scenario's script/global blocks and string constants and clear the
 * syntax nodes; otherwise commit via hs_compile_finish. Frees the compiled-source buffer(s) and marks the
 * compiler uninitialized. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/hs_compile_globals.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_block.h"
#include "headers/tag_data.h"
#include "headers/data_array.h"
extern uint8_t tag_block_resize(tag_block *block, int element_count);
extern uint8_t tag_data_resize(tag_data *data, int size);
extern void data_delete_all(data_array *data);
extern void hs_compile_finish(void);
extern void dlFree(void *ptr);

void hs_compile_dispose(void)
{
    if ( hs_compile_globals.permanent )
    {
        if ( hs_compile_globals.error_since_initialize )
        {
            scenario *scn = global_scenario;
            tag_block_resize(&scn->hs_scripts, 0);
            tag_block_resize(&scn->hs_globals, 0);
            tag_data_resize(&scn->hs_string_constants, 0);
            data_delete_all(hs_syntax_data);
        }
        else
        {
            hs_compile_finish();
        }
        if ( hs_compile_globals.compiled_source )
            dlFree(hs_compile_globals.compiled_source);
    }

    if ( hs_compile_globals.malloced )
    {
        dlFree(hs_compile_globals.compiled_source);
        hs_compile_globals.compiled_source = 0;
        hs_compile_globals.malloced = 0;
    }
    hs_compile_globals.initialized = 0;
}
