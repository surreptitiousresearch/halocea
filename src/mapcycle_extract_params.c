/* mapcycle_extract_params @0x83766C78 — splits one mapcycle config line "map, variant" in place at the
 * first comma, trims whitespace from both halves, and reports whether both a map name and a variant name
 * were present. */

#include <stdint.h>
#include <string.h>

extern void trim_whitespace(char **str);

uint8_t mapcycle_extract_params(char *str, char **map_name, char **variant_name)
{
    *map_name = 0;
    *variant_name = 0;

    char *comma = strchr(str, ',');
    if ( comma )
    {
        *comma = 0;
        *variant_name = comma + 1;
        *map_name = str;
        trim_whitespace(variant_name);
        trim_whitespace(map_name);
    }

    return *variant_name != 0 && *map_name != 0;
}
