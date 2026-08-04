#pragma once
/* DEVIATION: this header previously defined its own local `message_delta_processor_decoding_information`
 * (fields typed as plain `int`) duplicating and diverging from the dedicated, DB-resolved
 * message_delta_processor_decoding_information.h (which types `mode`/`definition_type` as their real
 * enums). No existing .c included both headers at once so the redefinition never surfaced, but it's a
 * latent conflict — fixed by including the dedicated header instead of redefining the struct here. */
#include "bitstream_t.h"
#include "message_delta_processor_decoding_information.h"

typedef struct message_delta_processor_header
{
    message_delta_processor_decoding_information *decoding_information; /* 0x00 */
    unsigned char  field_included[64];                                   /* 0x04 */
    void          *custom_header;                                        /* 0x44 */
} message_delta_processor_header;                                        /* 72 bytes */
