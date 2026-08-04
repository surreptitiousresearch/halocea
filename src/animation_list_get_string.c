#include <stdint.h>
#include "headers/animation_list.h"

char * animation_list_get_string(animation_list *list, int16_t index)
{
    if ( index >= list->count )
        return "#<invalid>";
    char *result = list->animations[index].name;
    if ( !result )
        return "#<invalid>";
    return result;
}
