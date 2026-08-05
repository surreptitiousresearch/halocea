/* message_begin_encoding @0x837A1F18 — tail-call forwarder
 * (`b message_write_iteration_independent_header_pre_encoding`; confirmed via disasm, not a bare blr).
 * message_write_iteration_independent_header_pre_encoding itself is not yet decompiled — declared
 * extern with its own DB prototype and called through with the same argument. */

#include <stdint.h>
#include "headers/iterated_message.h"

extern int message_write_iteration_independent_header_pre_encoding(iterated_message *message);

uint8_t message_begin_encoding(iterated_message *const message)
{
    return message_write_iteration_independent_header_pre_encoding(message);
}
