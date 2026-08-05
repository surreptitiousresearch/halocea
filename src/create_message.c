/* create_message @0x838105B0 — builds a message_header word in `buffer` (or a freshly allocated one if
 * `buffer` is null), then copies `data` after the header. Size field = data_size plus the header's own
 * 2 bytes; type field = type & MAXIMUM_MESSAGE_TYPE. Both flag bits are left clear (unencrypted), as in
 * the in-place counterpart prepare_buffer_to_send@0x83810638. */

#include <stdint.h>
#include "headers/message_header_flags.h"

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
        /* DEVIATION: Hex-Rays folded the field packing into `4 * ((4 * total_size) | (type & 3))`;
         * the disasm builds each field at its own DB-named width (insrwi r27,r30,16,14 then
         * clrlslwi r11,r27,18,2 @0x83810604-0x8381060C = 12-bit size at [15:4], 2-bit type at [3:2]). */
        *message = (uint16_t)(((uint32_t)(total_size & MAXIMUM_MESSAGE_SIZE) << _message_size_bit0)
                            | (((uint32_t)type & MAXIMUM_MESSAGE_TYPE) << _message_type_bit0));

        if ( data )
            memcpy(message + 1, data, data_size);
    }

    return message;
}
