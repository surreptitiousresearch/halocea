/* empty_wide_string @0x820309EC (.rdata) — the shared L"" wide string literal.
 *
 * The Blam UI's "tag not loaded" fallback: every `text_tag == -1 ? empty_wide_string :
 * unicode_string_list_get_string(...)` site returns it. Like empty_string it has no exported name;
 * it is the pooled anonymous `L""`, and 115 sites take its address (get_place_string @0x83746530,
 * player_new, credits_item_new, hud_enable_custom_state_message, …).
 *
 * Extent: IDA types 0x820309EC as a 16-bit word (`word_820309EC`) = one wchar_t; 0x820309EE is
 * unreferenced padding ahead of the 4-aligned L"<null>" at 0x820309F0. Value: 0x0000.
 * Segment .rdata (perm 4, read-only) -> const. MSVC wchar_t is 16-bit on both the Xbox 360 target
 * and x64, so the width is portable.
 */

#include <stddef.h>   /* wchar_t */

const wchar_t empty_wide_string[1] = L"";
