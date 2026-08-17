/* iterated_message_dispose @0x837A0C38 */
#include "headers/iterated_message.h"

#include "headers/bitstream_t.h"
extern int bitstream_dispose(bitstream_t *bit_stream);

int iterated_message_dispose(iterated_message *const message)
{
    message->initialized = 0;
    return bitstream_dispose(&message->iteration_independent_stream);
}
