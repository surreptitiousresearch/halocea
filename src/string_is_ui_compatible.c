/* string_is_ui_compatible @0x83766660 — checks that an ASCII string can be displayed and (for some
 * modes) entered through the Halo UI: every character must be printable, present in the small_ui font,
 * and accepted by the on-screen keyboard for the given validate_mode. Mode 3 additionally rejects an
 * empty string and a leading space. Mode 1 also requires the wide form to contain visible characters.
 * Returns nonzero if compatible. */

#include <stdint.h>
#include <stddef.h>   /* wchar_t */
#include "headers/global_tag_instances.h"
#include "headers/virtual_keyboard_validate_mode.h"

typedef struct font_header font_header;

extern int tag_loaded(uint32_t group_tag, const char *name);
typedef struct font_character font_character;
extern font_character *font_get_character_by_ascii_code(font_header *header, uint16_t character);
extern uint8_t virtual_keyboard_is_input_valid(int ascii_code, int validate_mode);
extern uint8_t string_has_visible_characters(const wchar_t *str);

uint8_t string_is_ui_compatible(const char *str, const wchar_t *wstr, int validate_mode)
{
    int compatible = 1;
    int font_tag = tag_loaded(0x666F6E74u /* 'font' */, "ui\\small_ui");
    font_header *font = TAG_GET(font_header, font_tag);

    const char *scan = str;
    while (*scan++)
        ;
    int length = scan - str - 1;

    if (validate_mode == _virtual_keyboard_validate_not_empty)
        compatible = *(uint8_t *)str != 0;

    if (compatible && length > 0)
    {
        int i = 0;
        while (1)
        {
            char c = str[i];
            if (c < 32 || c == 255   /* FAITHFUL-AS-BUILT (CS-1 sentinel-promotion sweep): `c` is signed char, so `== 255` can never be true — and the binary's compare is dead the same way: lbzx r11,r30,r31 @0x837666F0 + extsb r10,r11 @0x837666F4 + cmpwi cr6,r10,0xFF @0x83766700. A SIGN-extending narrow read against a positive 255, so the shipped build carries the same dead test; adding a (uint8_t) cast here would introduce behaviour the binary does not have. */
                || !font_get_character_by_ascii_code(font, c)
                || !virtual_keyboard_is_input_valid(str[i], validate_mode))
            {
                compatible = 0;
                break;
            }
            if (validate_mode == _virtual_keyboard_validate_not_empty)
            {
                bool valid_leading = true;
                if (!i)
                {
                    int first = *str;
                    if (first == 32 || first == -96)   /* space or non-breaking space */
                        valid_leading = false;
                }
                compatible = valid_leading;
                if (!valid_leading)
                    return compatible;
            }
            if (++i >= length)
                break;
        }
    }

    if (validate_mode != _virtual_keyboard_validate_profile_name)
        return compatible;
    if (!compatible)
        return 0;
    if (!string_has_visible_characters(wstr))
        return 0;
    return 1;
}
