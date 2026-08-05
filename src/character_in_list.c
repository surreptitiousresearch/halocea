#include <stdint.h>
int character_in_list(char c, int16_t list_count, const char *list)
{
    for (int16_t i = 0; i < list_count; ++i)
    {
        if (c == list[i])
            return 1;
    }
    return 0;
}
