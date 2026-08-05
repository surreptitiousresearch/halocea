/* finished_decoding @0x837A0EA0 — record decode-completion metrics for an iterated message: a rejection
 * (0 bits) if nothing was decoded, otherwise the successful decode with its iteration count. Returns
 * whether the decode succeeded. */

#include <stdint.h>
#include "headers/message_delta_processor_decoding_information.h"

extern _message_definition *message_delta_global_message_list[47];
extern void metrics_record_message_decode(const _message_definition *const message_definition, const message_delta_processor_mode mode, const int encoded_size_in_bits, const int iterations);

uint8_t finished_decoding(const message_delta_processor_decoding_information *decoding_information, int bits_decoded)
{
    const _message_definition *definition = message_delta_global_message_list[decoding_information->definition_type];

    if ( bits_decoded == 0 )
    {
        metrics_record_message_decode(definition, decoding_information->mode, 0, 0);
        return 0;
    }

    metrics_record_message_decode(definition, decoding_information->mode, bits_decoded, decoding_information->iteration_count);
    return 1;
}
