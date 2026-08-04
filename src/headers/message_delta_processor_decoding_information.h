#pragma once
/* message_delta_processor_decoding_information — decode-side counterpart to message_delta_processor_header;
 * tracks in-progress iterated-message decode state. Layout from the database (types_members). */

#include "message_delta_processor_mode.h"
#include "message_definition.h"
#include "bitstream_t.h"

typedef struct message_delta_processor_decoding_information
{
    message_delta_processor_mode                     mode;                      /* 0x00 */
    message_delta_processor_message_definition_type  definition_type;           /* 0x04 */
    int                                               iteration_count;          /* 0x08 */
    int                                               state;                    /* 0x0C */
    bitstream_t                                      *input_stream;             /* 0x10 */
    int                                               original_stream_position; /* 0x14 */
    int                                               current_iteration;         /* 0x18 */
    unsigned __int8                                   iteration_header_decoded;  /* 0x1C */
    unsigned __int8                                   iteration_body_decoded;    /* 0x1D */
    unsigned char _pad0[2]; /* db-verified padding */
    int                                               iteration_count_bits;      /* 0x20 */
    int                                               protocol_bits;             /* 0x24 */
    int                                               message_id_bits;           /* 0x28 */
    int                                               message_mode_bits;         /* 0x2C */
    int                                               padding_bits;              /* 0x30 */
} message_delta_processor_decoding_information; /* 52 bytes */
