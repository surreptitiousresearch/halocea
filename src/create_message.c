#include <stdint.h>
/* create_message @0x838105B0 — builds a network message header word (a 2-bit type tag packed with the
 * message's total size) in `buffer` (or a freshly allocated one if `buffer` is null), then copies `data`
 * after the header. */

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void *memcpy(void *dest, const void *src, unsigned int size);

uint16_t * create_message(uint8_t type, void *data, uint16_t data_size, uint16_t *buffer, uint16_t buffer_size)
{
    uint16_t total_size = data_size + 2;
    uint16_t *message = buffer
        ? buffer
        : (uint16_t *)dlMalloc(total_size, "D:\\Projects\\code\\HCEX\\sources\\bungie_net\\common\\message_header.c", 0x2Eu);

    if ( message )
    {
        *message = 4 * ((4 * total_size) | (type & 3));

        if ( data )
            memcpy(message + 1, data, data_size);
    }

    return message;
}
