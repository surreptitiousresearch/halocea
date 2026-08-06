/* hcex_unicode_string_list_get_string @0x823CF158 — hcex bridge: resolve one entry of a Blam
 * unicode_string_list tag to its ws-engine localized wide string. Looks up the tag's name, builds the
 * per-string lookup key via hcex_conv_str_name (name + string_index), and hands that key to hcex_get_string.
 * Returns nullptr when the tag index does not resolve to a name. */
#include <stdint.h>

extern "C" const char *hcex_tag_get_name(int id);
extern "C" char *hcex_conv_str_name(const char *name, int idx, char *out, int max_len);
extern wchar_t *hcex_get_string(const char *str_name);

extern "C" wchar_t *hcex_unicode_string_list_get_string(int tag_index, int16_t string_index)
{
    const char *tag_name = hcex_tag_get_name(tag_index);
    if ( tag_name )
    {
        char string_key[128];
        hcex_conv_str_name(tag_name, string_index, string_key, 127);
        return hcex_get_string(string_key);
    }
    return (wchar_t *)0;
}
