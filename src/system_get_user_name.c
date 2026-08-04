#include <stdint.h>
#include <string.h>

void system_get_user_name(char *name, int16_t maximum_length)
{
    strncpy(name, "xbox", maximum_length);
}
