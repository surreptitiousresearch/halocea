/* hcex_conv_cls_name @0x823F2990 — build the HCEX texture-manager / asset key for an object CLASS tag path.
 * Starts the key with the caller-supplied prefix (or empty when none), then folds the model tag path into a
 * short mnemonic: "weapons\\" -> "wpn_" + folder initials, "vehicles\\" -> "vhc_" + folder initials,
 * "characters\\" -> "char_", followed by the leaf (final path component). The composed key is normalized by
 * hcex_conv_name before return. Paths that match none of the three folder classes yield the bare prefix and
 * return null (no key built). For weapons/vehicles the "initials" are the first letter of the folder name
 * plus — when a space precedes the next backslash — the first letter of the word after that space.
 *
 * hcex_conv_name is an HCEX sibling; strncmp/strchr/strrchr/strcat are CRT boundaries. */

#include <string.h>

extern "C" char *hcex_conv_name(char *name, int max_len);

extern "C" char *hcex_conv_cls_name(const char *name, const char *name_prefix, char *out, int max_len)
{
    if ( !name )
        return 0;

    /* seed the key with the caller-supplied prefix (empty string when none) */
    if ( name_prefix )
    {
        const char *prefix_src = name_prefix;
        char ch;
        do
        {
            ch = *prefix_src;
            out[prefix_src - name_prefix] = ch;
            ++prefix_src;
        }
        while ( ch );
    }
    else
    {
        *out = 0;
    }

    if ( !strncmp(name, "weapons\\", 8) )
    {
        const char *space = strchr(name + 8, ' ');
        strcat(out, "wpn_");
        int key_len = (int)strlen(out);
        out[key_len] = name[8];              /* first folder-name initial */
        int cursor = key_len + 1;
        if ( space )
            out[cursor++] = space[1];        /* initial of the word after the space */
        out[cursor] = '_';
        out[cursor + 1] = 0;
        const char *leaf = strrchr(name, '\\') + 1;
        strcat(out, leaf);
        return hcex_conv_name(out, max_len);
    }

    if ( !strncmp(name, "vehicles\\", 9) )
    {
        const char *space = strchr(name + 9, ' ');
        strcat(out, "vhc_");
        int key_len = (int)strlen(out);
        out[key_len] = name[9];              /* first folder-name initial */
        int cursor = key_len + 1;
        if ( space )
            out[cursor++] = space[1];        /* initial of the word after the space */
        out[cursor] = '_';
        out[cursor + 1] = 0;
        const char *leaf = strrchr(name, '\\') + 1;
        strcat(out, leaf);
        return hcex_conv_name(out, max_len);
    }

    if ( !strncmp(name, "characters\\", 11) )
    {
        strcat(out, "char_");
        const char *leaf = strrchr(name, '\\') + 1;
        strcat(out, leaf);
        return hcex_conv_name(out, max_len);
    }

    return 0;
}
