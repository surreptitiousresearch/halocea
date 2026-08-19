/* prepare_buffer_to_send @0x83810638 — stamp the 2-byte message_header word at the head of an
 * already-filled buffer (the in-place counterpart of create_message@0x838105B0). Size field =
 * content_size plus the header's own 2 bytes; type field = type & MAXIMUM_MESSAGE_TYPE (one of
 * _message_type_unknown/_error/_data/_packet); both flag bits are left clear, i.e. unencrypted.
 * Returns `buffer` unchanged — not a pointer past the header — and stores nothing when null.
 * The lone 16-bit store lands the word big-endian = network order on this target, which is why
 * _byte_swap_message_header@0x838105A8 is a bare-blr no-op; on a LE host it would have to be real. */

#include <stdint.h>
#include "headers/message_header_flags.h"

uint16_t * prepare_buffer_to_send(uint8_t type, void *buffer, uint16_t content_size)
{
    if (buffer)
    {
        uint32_t message_size = (uint32_t)content_size + NUMBER_OF_MESSAGE_HEADER_BITS / 8;

        /* DEVIATION: Hex-Rays folded the field packing into `16 * (content_size + 2) | ((4 * type) & 0xC)`;
         * the disasm masks each field to its own DB-named width first (clrlslwi r8,r9,20,4 = 12-bit size,
         * clrlslwi r10,r3,30,2 = 2-bit type). */
        *(uint16_t *)buffer = (uint16_t)(((message_size & MAXIMUM_MESSAGE_SIZE) << _message_size_bit0)
                                       | (((uint32_t)type & MAXIMUM_MESSAGE_TYPE) << _message_type_bit0));
    }
    return (uint16_t *)buffer;
}
