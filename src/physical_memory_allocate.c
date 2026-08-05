/* physical_memory_allocate @0x836E0508 — reserve the physical memory regions used at boot.
 *
 * Commits a 0x1448000-byte block at fixed address 0x70000 for the game-state base, registers it with
 * the HCEX memory debugger, sets the tag-cache base address to 0x4B8000, and zero-fills the first
 * 0x448000 bytes of the game-state region. */

#include "headers/physical_memory_map_globals.h"
#include <string.h>

/* Win32/Xbox boundary. */
extern void *VirtualAlloc(void *lpAddress, unsigned int dwSize, unsigned int flAllocationType,
                          unsigned int flProtect);
extern void hcex_memory_debug_add(void *address, int size, const char *file, int line);

void physical_memory_allocate(void)
{
    physical_memory_map_globals.game_state_base_address = 0;
    physical_memory_map_globals.tag_cache_base_address = 0;

    physical_memory_map_globals.game_state_base_address =
        VirtualAlloc((void *)0x70000, 0x1448000u, 0x3000u, 4u);
    hcex_memory_debug_add(
        physical_memory_map_globals.game_state_base_address,
        0x1448000,
        "D:\\Projects\\code\\HCEX\\sources\\cache\\physical_memory_map.c",
        88);

    physical_memory_map_globals.tag_cache_base_address = (void *)0x4B8000;
    memset(physical_memory_map_globals.game_state_base_address, 0, 0x448000u);
}
