/* is_filtered_tag @0x836B9F28 — true if the given tag's name is on the FMOD debug system's skip filter.
 *
 * The skip check dispatches to the Saber/FMOD sound side — snd::SYSTEM_DBG_FMOD::IsSkipped(&snd::SystemFMod->dbg, name)
 * in the binary. That is a mangled C++ boundary from Blam's perspective, so it is declared here as an
 * extern C boundary shim. */

#include <stdint.h>

extern char *tag_get_name(int tag_index);
/* boundary: snd::SYSTEM_DBG_FMOD::IsSkipped(&snd::SystemFMod->dbg, name) */
extern int snd_system_dbg_is_skipped(const char *name);

int is_filtered_tag(int tag)
{
    const char *name = tag_get_name((short)tag);
    return snd_system_dbg_is_skipped(name);
}
