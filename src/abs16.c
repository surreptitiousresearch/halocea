/* abs16 — signed 16-bit absolute value. Inline-only helper: it has NO entry of its own in the
 * image, so every address below is an interior citation of another function's body.
 *   @0x8379FD48 (inlined in hud_draw_numbers)         srawi r10,r11,0x1F / xor r8,r11,r10 / subf r11,r10,r8
 *   @0x837699F8 (inlined in text_pick_draw_character) srawi r31,r8,0x1F / xor r8,r8,r31 / subf r8,r31,r8
 *
 * DEVIATION: the binary has no out-of-line abs16 — it is the standard branchless integer-abs idiom
 * emitted inline at each site. The value is first sign-extended to 32 bits (extsh), then:
 *   mask = value >> 31   (srawi, arithmetic: 0 if non-negative, -1 if negative)
 *   result = (value ^ mask) - mask
 * which yields value when non-negative and -value when negative. Restored here as a faithful pure
 * function; the extsh back to 16 bits at each call site is the (__int16) return truncation. */

#include <stdint.h>

int16_t abs16(int16_t value)
{
    int32_t sign_extended = value;                          /* extsh: sign-extend __int16 to 32-bit */
    int32_t mask = sign_extended >> 31;                     /* srawi rX,rX,0x1F: 0 or -1 */
    return (int16_t)((sign_extended ^ mask) - mask);        /* xor then subf: branchless abs */
}
