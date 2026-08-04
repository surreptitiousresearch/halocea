#include <stdint.h>
extern struct { int pic_name_index; } multiplayer_maps[];

int16_t map_list_get_pic_name_index(int index)
{
    return multiplayer_maps[index].pic_name_index;
}
