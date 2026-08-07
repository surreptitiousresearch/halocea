/* hcex_conv_mdl_name @0x823F2620 — build the HCEX key for a MODEL tag path. Normally the key is just the
 * leaf (final path component). For first-person view models whose path ends in "\\fp\\fp", the key is the
 * parent folder name joined with "\\fp" (walk back to the backslash preceding the "\\fp\\fp" suffix, copy
 * through the second backslash of that suffix). For level scenario models ("levels\\<name>\\...") the level
 * folder name is appended. The result is normalized by hcex_conv_name before return.
 *
 * hcex_conv_name is an HCEX sibling; strncmp/strchr/strrchr/strncpy/strncat are CRT boundaries. */

#include <string.h>

extern "C" char *hcex_conv_name(char *name, int max_len);

extern "C" char *hcex_conv_mdl_name(const char *mdl_name, char *out, int max_len)
{
    const char *end = mdl_name;
    while ( *end++ )
        ;                                    /* end points one past the terminating null */
    int name_len = (int)(end - mdl_name - 1);
    const char *fp_suffix = end - 7;         /* start of a trailing "\\fp\\fp" if present */

    const char *copy_start;
    const char *copy_end;
    if ( strncmp(fp_suffix, "\\fp\\fp", 6) )
    {
        /* ordinary model: key is the leaf component */
        copy_start = strrchr(mdl_name, '\\') + 1;
        copy_end   = &mdl_name[name_len];
    }
    else
    {
        /* first-person model: parent folder + "\\fp" */
        copy_start = fp_suffix - 1;
        if ( copy_start > mdl_name )
        {
            while ( copy_start[-1] != '\\' )
            {
                --copy_start;
                if ( (uintptr_t)copy_start <= (uintptr_t)mdl_name )
                    break;
            }
        }
        copy_end = fp_suffix + 3;            /* through the second backslash of "\\fp\\fp" */
    }

    int copy_len = (int)(copy_end - copy_start);
    strncpy(out, copy_start, copy_len);
    out[copy_len] = 0;

    if ( !strncmp(mdl_name, "levels\\", 7) )
    {
        const char *level_sep = strchr(mdl_name + 7, '\\');
        strncat(out, mdl_name + 6, level_sep - mdl_name - 6);
    }

    return hcex_conv_name(out, max_len);
}
