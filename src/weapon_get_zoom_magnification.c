/* weapon_get_zoom_magnification @0x836D9218 — magnification at a given zoom level, geometrically interpolated
 * between the weapon definition's minimum and maximum magnification across its zoom-level count:
 * magnification = minimum * (maximum/minimum) ^ (zoom_level / (count - 1)). Non-positive min/max
 * default to 1.0; out-of-range or negative zoom levels yield 1.0.
 *
 * DEVIATION: the decompiler exposed the PPC soft-float pow() argument pair as phantom parameters (a3/a4/a5) and
 * a raw __SPAIR64__ division; reconstructed as the standard geometric interpolation. */

#include <math.h>
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/blam_data_globals.h"


float weapon_get_zoom_magnification(int weapon_index, int16_t zoom_level)
{
    float magnification = 1.0f;
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    if ( zoom_level >= 0 )
    {
        int16_t zoom_level_count = definition->weapon.zoom_level_count;
        if ( zoom_level < zoom_level_count )
        {
            float fraction = zoom_level_count <= 1 ? 0.0f : (float)zoom_level / (float)(zoom_level_count - 1);

            float minimum = definition->weapon.zoom_magnification_minimum;
            if ( minimum <= 0.0f )
                minimum = 1.0f;
            float maximum = definition->weapon.zoom_magnification_maximum;
            if ( maximum <= 0.0f )
                maximum = 1.0f;

            magnification = powf(maximum / minimum, fraction) * minimum;
        }
    }
    return magnification;
}
