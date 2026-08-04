/* message_delta_processor_end_iteration @ 0x837A0928 — Blam engine.
 * Closes out one iteration of an iterated message-delta decode: returns whether
 * both the iteration header and body were decoded, advances the iteration index,
 * clears the per-field inclusion flags, and resets the header/body-decoded state
 * for the next iteration. */

#include "headers/message_delta_processor_header.h"

#include <string.h>

int message_delta_processor_end_iteration(message_delta_processor_header *const header)
{
    message_delta_processor_decoding_information *decoding_information = header->decoding_information;

    int fully_decoded = (decoding_information->iteration_header_decoded == 1
                         && decoding_information->iteration_body_decoded == 1);

    ++decoding_information->current_iteration;
    memset(header->field_included, 0, sizeof(header->field_included));
    decoding_information->iteration_header_decoded = 0;
    decoding_information->iteration_body_decoded = 0;

    return fully_decoded;
}
