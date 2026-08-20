#include <stdint.h>
/* virtual_keyboard_launch @0x83783400 — retail stub. The entire function is
 * `li r3, 1` @0x83783400 / `blr` @0x83783404.
 *
 * CAVEAT — faithfully reconstructed; the whole subsystem is dead in the shipped binary. This is the
 * only entry point that would raise the on-screen keyboard, and it touches none of its three
 * arguments and never sets `virtual_keyboard_globals.active`. Every write to that byte across
 * src/blam/ (virtual_keyboard_initialize / _cancel / _dispose / _select) CLEARS it, so
 * `virtual_keyboard_active` can never return true and _process / _render never run — the layout
 * table, tab walks, character resolution and validation modes are all complete but unreachable.
 * Returning 1 makes callers believe the keyboard was raised. Do not "fix" this. */

uint8_t virtual_keyboard_launch(uint16_t *text_buffer, uint16_t buffer_size, int16_t caption_index)
{
    return 1;
}
