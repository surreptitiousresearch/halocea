#include <stdint.h>
/* saved_game_file_get_type @0x83726B58 — the saved-game type is packed into the low 4 bits of the
 * enumerated index (confirmed via disasm: `clrlwi r3, r3, 28` masks to the low 4 bits). */

uint16_t saved_game_file_get_type(int index)
{
    return (uint16_t)(index & 0xF);
}
