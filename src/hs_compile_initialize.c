/* hs_compile_initialize @0x83775C38 — reset the HaloScript compiler state for a new compile pass. When
 * `permanent` (a full scenario recompile) is set, also clear the scenario's compiled script/global/reference
 * blocks and string-constant data and empty the syntax-node array.
 *
 * Deviation: hs_compile_globals has no database type; see hs_compile_globals.h (fields derived from usage). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/hs_compile_globals.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_block.h"
#include "headers/tag_data.h"
#include "headers/data_array.h"
#include "headers/tag_groups.h"
extern void data_delete_all(data_array *data);

void hs_compile_initialize(uint8_t permanent)
{
    hs_compile_globals.initialized = 1;
    hs_compile_globals.compiled_source = 0;
    hs_compile_globals.compiled_source_size = 0;
    hs_compile_globals.permanent = permanent;
    hs_compile_globals.error_since_initialize = 0;
    hs_compile_globals.__noop = 0;
    if ( permanent )
    {
        scenario *scn = global_scenario;
        tag_block_resize(&scn->hs_scripts, 0);     /* +0x49C */
        tag_block_resize(&scn->hs_globals, 0);      /* +0x4A8 */
        tag_block_resize(&scn->hs_references, 0);   /* +0x4B4 */
        tag_data_resize(&scn->hs_string_constants, 0); /* +0x488 */
        data_delete_all(hs_syntax_data);
    }
}
