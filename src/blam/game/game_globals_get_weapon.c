/* game_globals_get_weapon @0x83745F58 — reads dword 3 (byte offset 0xC) of the indexed 16-byte
 * weapon_list element (confirmed via decompiler: `*((_DWORD*)game_globals->weapon_list.address + 4*index + 3)`). */

#include "headers/game_globals_definition.h"
#include "headers/tag_reference.h"

int game_globals_get_weapon(struct game_globals *game_globals, int index) /* struct tag: ordinary name game_globals is the runtime pointer variable */
{
    return ((tag_reference *)game_globals->weapon_list.address)[index].index;
}
