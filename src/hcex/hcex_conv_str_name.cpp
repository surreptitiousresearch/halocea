/* hcex_conv_str_name @0x823F2D50 — build an HCEX key for a tag path, folding the directory portion into a
 * compact mnemonic and appending a numeric suffix. For each "\\"-separated directory component the first
 * character is taken (so "sound\\sfx\\weapons\\..." contributes "ssw..."), EXCEPT when the path begins with
 * "levels\\" — there the full name of the component after "levels\\" is copied verbatim. A '_' separates the
 * folded directory from the leaf (final component), then "_<idx>" is appended. The result is normalized by
 * hcex_conv_name before return.
 *
 * hcex_conv_name is an HCEX sibling; sprintf_0 / strncmp / strchr / strcat are CRT boundaries. */

#include <string.h>

extern char *hcex_conv_name(char *name, int max_len);
extern int sprintf_0(char *string, const char *format, ...);

extern "C" char *hcex_conv_str_name(const char *name, int idx, char *out, int max_len)
{
    char index_suffix[112];

    *out = 0;
    const char *name_cursor = name;
    int out_len = (int)strlen(out);          /* 0 */

    for ( const char *sep = strchr(name, '\\'); sep; sep = strchr(sep + 1, '\\') )
    {
        if ( strncmp(name_cursor, "levels\\", 7) )
        {
            /* non-level component: contribute its first character */
            out[out_len++] = *name_cursor;
        }
        else
        {
            /* level component: copy the whole folder name after "levels\\" */
            sep = strchr(name_cursor, '\\') + 1;
            int component_char = (unsigned char)*sep;
            if ( !*sep )
            {
                name_cursor = sep;
                break;
            }
            do
            {
                if ( component_char == '\\' )
                    break;
                out[out_len++] = component_char;
                component_char = (unsigned char)*++sep;
            }
            while ( *sep );
            if ( !component_char )
            {
                name_cursor = sep;
                break;
            }
        }
        name_cursor = sep + 1;
    }

    out[out_len] = '_';
    out[out_len + 1] = 0;

    /* append the leaf (remaining) component */
    strcat(out, name_cursor);

    /* append "_<idx>" */
    sprintf_0(index_suffix, "_%d", idx);
    strcat(out, index_suffix);

    return hcex_conv_name(out, max_len);
}
