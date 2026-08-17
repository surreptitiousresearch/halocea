/* is_ansi_char @0x8376D930 */
#include <stdint.h>
int is_ansi_char(uint16_t wide_char)
{
    return (wide_char & 0xFF00) == 0;
}
