#include <stdint.h>
/* new_campaign_entered_name @ 0x8441E664 - .data zero-fill (IDA word_8441E664, no PDB name).
 * The virtual-keyboard text buffer for a new campaign profile name. new_campaign_chosen passes it to
 * ustrncpy with count 0xB (disasm 0x836975B4/0x836975C0) and to virtual_keyboard_launch with buffer_size
 * 0x18 (0x836975E0), i.e. 11 wide characters + NUL = 24 bytes, ending exactly at new_campaign_name_cursor
 * (0x8441E67A). */

uint16_t new_campaign_entered_name[11];
