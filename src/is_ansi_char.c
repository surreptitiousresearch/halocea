#include <stdint.h>
int is_ansi_char(uint16_t wide_char)
{
    return (wide_char & 0xFF00) == 0;
}
