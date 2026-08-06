/* hcex_conv_eff_name @0x823F2730 — derive a candidate sfx brand name from a model tag path. Picks a prefix
 * from the model's top-level folder ("weapons\\" -> "sfx_" + initials, "vehicles\\" -> "sfx_vhc_" + initials,
 * "characters\\" -> "sfx_cc_", anything else -> a default 5-char prefix), appends the model's leaf name, then
 * hands the result to hcex_conv_name for final normalization.
 *
 * For weapons/vehicles the "initials" are the first letter of the model name plus — when a space precedes the
 * next backslash — the first letter of the word after that space, joined by '_'. */

#include <string.h>

extern "C" char *hcex_conv_name(char *name, int max_len);

extern "C" char *hcex_conv_eff_name(const char *mdl_name, char *out, int max_len)
{
    int name_len = (int)strlen(mdl_name);
    const char *leaf;
    const char *end;
    int prefix_len;

    out[0] = 0;

    if ( !strncmp(mdl_name, "weapons\\", 8) )
    {
        const char *space = strchr(mdl_name + 8, ' ');
        const char *token = (strchr(mdl_name + 8, '\\') >= space) ? space : 0;
        strcpy(out, "sfx_");
        strncat(out, mdl_name + 8, 1);
        if ( token )
        {
            out[5] = token[1];
            out[6] = '_';
            out[7] = 0;
        }
        else
        {
            out[5] = '_';
            out[6] = 0;
        }
    }
    else if ( !strncmp(mdl_name, "vehicles\\", 9) )
    {
        const char *space = strchr(mdl_name + 9, ' ');
        const char *token = (strchr(mdl_name + 9, '\\') >= space) ? space : 0;
        char initial = token ? token[1] : mdl_name[10];
        strcpy(out, "sfx_vhc_");
        strncat(out, mdl_name + 9, 1);
        out[9] = initial;
        out[10] = '_';
        out[11] = 0;
    }
    else if ( !strncmp(mdl_name, "characters\\", 11) )
    {
        strcpy(out, "sfx_cc_");
    }
    else
    {
        /* default prefix "sfx_" (copies aSfx_1[0..4] = "sfx_\0"; the decompiler rendered the
         * copy loop as algn_8200506A[i+1] by anchoring one byte into the align padding). */
        strcpy(out, "sfx_");
    }

    leaf = strrchr(mdl_name, '\\') + 1;
    end = &mdl_name[name_len];
    prefix_len = (int)strlen(out);
    strncat(out, leaf, end - leaf);
    out[prefix_len + (end - leaf)] = 0;
    return hcex_conv_name(out, max_len);
}
