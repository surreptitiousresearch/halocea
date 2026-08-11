/* tag_get_name @ 0x836F7520
   Returns the path/name string of a tag by index. */

#include <stdint.h>

#include "headers/cache_file_tag_instance.h"

/* DEVIATION: the parameter is a full word in the ABI and the narrowing is the callee's own.
   `extsh r8, r3` @0x836F7524 is this function sign-extending its OWN argument before the
   `slwi r10, r8, 5` stride, which it would not need if the caller had already narrowed; the DB
   prototype agrees (`char *tag_get_name(int tag_index)`). Callers pass whole words -- `lwz r3,
   4(r31)` @0x83681F78 in hcex_init_light and `lwz r3, global_scenario_index@l(r11)` @0x83684D00
   in game_state_header_valid are plain 32-bit loads, and cheat_by_model forwards tag_iterator_next's
   result under a full-word `cmpwi r3, -1` @0x8369A168. The old int16_t parameter narrowed at every
   call site instead, one instruction earlier than the binary does. The extsh is kept explicit as
   the (int16_t) below: sign-extending, per the CS-2 per-site rule, never a 0xFFFF mask. */
char *tag_get_name(int tag_index)
{
    return global_tag_instances[(int16_t)tag_index].name;
}
