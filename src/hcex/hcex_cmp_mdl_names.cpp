/* hcex_cmp_mdl_names @0x823CE3A0 — compare two MODEL tag paths by their HCEX keys. Both names are run
 * through hcex_conv_mdl_name (leaf component, or "<parent>\\fp" for first-person models, or with a
 * "levels\\<name>" prefix for scenario models) and the results are considered a match if either
 * converted key is a substring of the other. Used as a fuzzy comparator (e.g. qsort/bsearch-style
 * lookup), not a strict ordering.
 *
 * hcex_conv_mdl_name is an HCEX sibling; strstr is a CRT boundary. */

#include <string.h>

extern "C" char *hcex_conv_mdl_name(const char *mdl_name, char *out, int max_len);

extern "C" int hcex_cmp_mdl_names(const char *n1, const char *n2)
{
    char converted2[64];
    char converted1[64];

    if ( !n1 || !n2 )
        return 0;

    hcex_conv_mdl_name(n1, converted1, 64);
    hcex_conv_mdl_name(n2, converted2, 64);

    return strstr(converted1, converted2) != 0 || strstr(converted2, converted1) != 0;
}
