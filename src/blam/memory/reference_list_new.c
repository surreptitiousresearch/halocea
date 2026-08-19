/* reference_list_new @0x837752C8 — allocate a generic reference-list pool named "<name> reference", with
 * 12-byte elements (datum header + referenced datum index + next reference index). */

#include <stdint.h>
#include "headers/data_array.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern int sprintf_0(char *string, const char *format, ...);

data_array *reference_list_new(const char *name, int16_t maximum_count)
{
    char pool_name[256];

    sprintf_0(pool_name, "%s reference", name);
    return game_state_data_new(pool_name, maximum_count, 12);
}
