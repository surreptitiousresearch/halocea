/* build_message_header — pack a 16-bit message_header word from its fields: flags in bits 0-1
 * (encrypted / new-encryption-key), message type in bits 2-3, message size (length) in bits 4-15.
 *
 * DEVIATION: the decompiler renders the pack as nested `4 * (...)` multiplies; rewritten as shifts by
 * the named message_header_flags bit indices (equivalent: length<<4 | (type&3)<<2 | flags). */

#include <stdint.h>
#include "headers/message_header_flags.h"

void build_message_header(uint16_t *msg, uint16_t length, uint8_t type, uint8_t flags)
{
    *msg = (unsigned short)(((unsigned int)length << _message_size_bit0)
                          | (((unsigned int)type & MAXIMUM_MESSAGE_TYPE) << _message_type_bit0)
                          | flags);
}
