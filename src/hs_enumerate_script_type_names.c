#include <stdint.h>
#include "headers/blam_data_globals.h"
/* hs_enumerate_script_type_names @0x8372F3C0 — tail-call forwarder (confirmed via disasm, not a bare blr).
 * Enumerates the 5-entry hs_script_type_names string list starting at index 0. */

extern void hs_enumerate_from_string_list(const char **strings, int16_t start_index, int16_t count);

void hs_enumerate_script_type_names(void)
{
    hs_enumerate_from_string_list(hs_script_type_names, 0, 5);
}
