#include <stdint.h>
/* bitmap_format_type_valid_depth @0x836F4C58 — validate one bitmap tag field against the format/type
 * pair. Disasm 0x836F4C58-0x836F4C94: only the first two arguments are read. The second must be in
 * (0, 256]; then either it equals 1, or the first equals 1. The third argument is never touched.
 * Returns 0/1 zero-extended to 8 bits (clrlwi r3, r11, 24 @0x836F4C84 / @0x836F4C90), matching the DB
 * prototype's `unsigned __int8`.
 *
 * CAVEAT: the DB parameter names for this validator family are boilerplate and do NOT match register
 * usage, so the two literals below are deliberately left un-named rather than guessed. The siblings
 * bitmap_format_type_valid_width @0x836F4C08 and bitmap_format_type_valid_height @0x836F4C30 are both
 * prototyped (format, type, width|height) yet bound their FIRST argument by 30000 — the enum oracle's
 * MAXIMUM_BITMAP_WIDTH / MAXIMUM_BITMAP_HEIGHT, a bound no bitmap *format* enum could carry. Here the
 * bound is 256 = MAXIMUM_BITMAP_DEPTH, applied to the SECOND argument, and the `== 1` compare would fit
 * either _bitmap_format_y8 or _bitmap_type_3d. The function has zero callers in the binary, so nothing
 * disambiguates the true parameter order; naming the literals is escalated, not assumed. */

uint8_t bitmap_format_type_valid_depth(int16_t format, int16_t type, int16_t depth)
{
    return type > 0 && type <= 256 && (type == 1 || format == 1);
}
