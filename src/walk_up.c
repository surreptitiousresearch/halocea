/* walk_up @0x83810EF0 — advance one frame up the saved-EBP stack-walk chain. Returns the return
 * address stored in the current frame ([frame+4]); then follows the saved-frame link ([frame]),
 * accepting it only if it is 4-byte-aligned and monotonically increasing (>= old_ebp), else the
 * walk terminates by nulling the current frame. Both the current-frame cursor and old_ebp are
 * updated for the next call. */

#include "headers/blam_data_globals.h"

unsigned int walk_up(void)
{
    unsigned int result = 0;

    if ( walk_up_current_frame )
    {
        /* walk_up_current_frame is a DB-unnamed dword (0x844AB54C) holding a raw
         * frame address; the frame itself has no DB struct — [0]=saved link,
         * [1]=return address — so index through a typed local, exact bytes kept. */
        unsigned int *frame = (unsigned int *)walk_up_current_frame;
        result = frame[1];
        unsigned int next_frame = frame[0];

        char valid = 1;
        if ( (next_frame & 3) != 0 || next_frame < (unsigned int)old_ebp )
            valid = 0;

        walk_up_current_frame = valid ? next_frame : 0;
        old_ebp = (unsigned int *)walk_up_current_frame;
    }

    return result;
}
