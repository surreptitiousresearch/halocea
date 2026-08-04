/* hs_dispose_from_old_map @ 0x83730078-adjacent (0x837300E8) — release HaloScript syntax
 * pool (if self-allocated) and tear down runtime + object lists. */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include "headers/data_array.h"
extern void hs_node_gc(void);
extern void data_make_invalid(data_array *data);
extern void data_dispose(data_array *data);
extern void hs_runtime_dispose_from_old_map(void);
extern void object_lists_dispose_from_old_map(void);
void hs_dispose_from_old_map(void)
{
    if ( hs_syntax_data )
    {
        hs_node_gc();
        if ( hs_syntax_data_allocated )
        {
            data_make_invalid(hs_syntax_data);
            data_dispose(hs_syntax_data);
            hs_syntax_data_allocated = 0;
        }
        hs_syntax_data = 0;
    }
    hs_runtime_dispose_from_old_map(); /* attested void: r3-thread broken, flattened */
    object_lists_dispose_from_old_map(); /* attested void: return-thread + phantom arg dropped */
}
