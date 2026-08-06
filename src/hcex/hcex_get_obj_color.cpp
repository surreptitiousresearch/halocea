/* hcex_get_obj_color @0x83681B78 — pack a live Blam object's change-color tint into a 0x00RRGGBB integer.
 * Resolves the object body via the object_header datum array (shared 3*(u16)id+2 dword idiom), reads its
 * three color floats (object fields 98/99/100 == +0x188/+0x18C/+0x190), scales each by 255 and packs
 * R<<16 | G<<8 | B. The Blam object body has no DB struct applied, so the RGB triple is a raw float read. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/object_datum.h"

extern "C" data_array *object_header_data;

extern "C" int hcex_get_obj_color(int id)
{
    object_datum *object = (object_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;

    /* object+392/396/400 = base_change_colors[0].{red,green,blue} */
    unsigned int red   = (unsigned char)(int)(object->object.base_change_colors[0].red   * 255.0f);
    unsigned int green = (unsigned char)(int)(object->object.base_change_colors[0].green * 255.0f);
    unsigned int blue  = (unsigned char)(int)(object->object.base_change_colors[0].blue  * 255.0f);

    return (red << 16) | (green << 8) | blue;
}
