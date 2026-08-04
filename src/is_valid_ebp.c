#include <stdint.h>
#include "headers/blam_data_globals.h"
/* is_valid_ebp @0x83810EB8 — sanity-checks the saved frame pointer used by the debug stack walker: it must
 * be 4-byte aligned and no lower than the process's initial stack base (`old_ebp`, presumably named for the
 * x86 tool origin of this stack-walk code ported to PPC). */

extern unsigned int walk_up_current_frame;

uint8_t is_valid_ebp(void)
{
    if ( (walk_up_current_frame & 3) != 0 )
        return 0;
    /* old_ebp holds the initial stack base; compared by value (not dereferenced) */
    return walk_up_current_frame >= (unsigned int)old_ebp;
}
