/* hs_hack @0x837302F8 — dev/tool path: rebuild scripts from disk, recompile, and reinitialize HaloScript
 * for the current map if rebuild succeeded.
 *
 * DEVIATION: the decompiler threads hs_compile_source()'s and hs_dispose_from_old_map()'s return values in
 * as arguments to hs_dispose_from_old_map/hs_initialize_for_new_map, but both are established elsewhere in
 * this codebase (hs_dispose_from_old_map.c, hs_initialize_for_new_map.c) as taking no parameters — a
 * leftover-register artifact of the decompiler, not real arguments. */

#include <stdint.h>

extern uint8_t hs_rebuild_source(void);
extern uint8_t hs_compile_source(void);
extern void hs_dispose_from_old_map(void); /* attested void */
extern void hs_initialize_for_new_map(void);

void hs_hack(void)
{
    if ( hs_rebuild_source() )
    {
        hs_compile_source();
        hs_dispose_from_old_map();
        hs_initialize_for_new_map(); /* attested void: r3-thread flattened */
    }
}
