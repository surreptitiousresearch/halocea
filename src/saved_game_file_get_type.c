#include <stdint.h>
/* saved_game_file_get_type @0x83726B58 — the saved-game type is packed into the low 4 bits of the
 * enumerated index (confirmed via disasm: `clrlwi r3, r3, 28` masks to the low 4 bits). */

uint16_t saved_game_file_get_type(int index)
{
    return (unsigned __int16)(index & 0xF);
}
