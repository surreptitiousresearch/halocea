/* model_find_marker @0x83798F94 — binary-search a model (gbxmodel) tag's marker array for a marker by name
 * (case-insensitive). Returns the marker index, or -1 if not found / bad args. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/model.h"
#include "headers/model_marker.h"

extern int stricmp(const char *a, const char *b);

int16_t model_find_marker(int model_index, const char *name)
{
    if ( model_index == -1 )
        return -1;
    if ( !name )
        return -1;
    if ( !*name )
        return -1;

    __int16 low = 0;
    model *model_definition = TAG_GET(model, model_index);
    __int16 high = (unsigned __int16)model_definition->markers.count - 1;
    if ( high < 0 )
        return -1;

    while ( 1 )
    {
        int mid = (__int16)((low + high) / 2);
        int cmp = stricmp(name, ((model_marker *)model_definition->markers.address)[mid].name);
        if ( !cmp )
            return mid;
        if ( cmp >= 0 )
            low = mid + 1;
        else
            high = mid - 1;
        if ( low > high )
            return -1;
    }
}
