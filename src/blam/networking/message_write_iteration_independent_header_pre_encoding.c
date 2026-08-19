/* message_write_iteration_independent_header_pre_encoding @0x837A15D0 — write the per-message header bits
 * that precede all iterations: the mode flag (1 bit), the message id (message_id_bits, base 6), and — when
 * protocol changeover is enabled — the changeover flag plus the protocol id. Each write's success is ANDed
 * into the returned status. message_mode_bits/message_id_bits/protocol_bits are updated to the bit costs.
 *
 * DEVIATION: the decompiler's (vN = 1, !ok) comma idioms are reconstructed here as plain `&&` of the write
 * result with the running success flag, which is equivalent. */

#include <stdint.h>
#include "headers/iterated_message.h"
#include "headers/blam_data_globals.h"


#include "headers/bitstream_t.h"
#include "headers/bitstream_t.h"
#include "headers/bitstream_t.h"
extern unsigned int message_delta_parameters_protocol_get(void);
extern int message_delta_parameters_protocol_get_packet_bit_size(void);
extern unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value);
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

int message_write_iteration_independent_header_pre_encoding(iterated_message *message)
{
    int protocol = message_delta_parameters_protocol_get();

    unsigned int wrote_mode = bitstream_write_bit(&message->iteration_independent_stream, message->mode);
    int message_id_bits = message->message_id_bits + 6;
    message->message_mode_bits = 1;
    message->message_id_bits = message_id_bits;

    uint8_t ok = (bitstream_write_bits(&message->iteration_independent_stream,
                                               &message->definition_type, message_id_bits)
                          && wrote_mode) ? 1 : 0;

    if ( g_protocol_changeover_enabled != 1 )
        return ok;

    message->protocol_bits = message_delta_parameters_protocol_get_packet_bit_size();

    uint8_t ok2 = (bitstream_write_bit(&message->iteration_independent_stream,
                                               g_protocol_changeover_message_flag)
                           && ok) ? 1 : 0;
    uint8_t ok3 = (bitstream_write_bits(&message->iteration_independent_stream, &protocol,
                                                message->protocol_bits)
                           && ok2) ? 1 : 0;
    ++message->protocol_bits;
    return ok3;
}
