/* tag_group_to_widget_type @0x83755F48 — linear-searches widget_type_definitions for the entry whose
 * group_tag matches, returning its index (or -1 if none of the 5 object widget types match). */

#include "headers/widget_type_definition.h"

int tag_group_to_widget_type(unsigned int group_tag)
{
    for (int i = 0; i < 5; i++)
    {
        if (widget_type_definitions[i].group_tag == group_tag)
            return i;
    }
    return -1;
}
