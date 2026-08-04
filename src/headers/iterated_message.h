#pragma once
/* iterated_message — the working state of an in-progress message-delta encode/decode: the target buffer and
 * bit budget, the iteration-independent header bitstream, and per-iteration header/field-map/data bitstreams.
 * Driven by encode_iterations. */

#include "bitstream_t.h"
#include "message_definition.h"
#include "message_delta_processor_mode.h"  /* was duplicated inline; use the sibling definition */

typedef struct iterated_message
{
    unsigned __int8                                 initialized;                   /* 0x00 */
    unsigned char _pad0[3]; /* db-verified padding */
    message_delta_processor_message_definition_type definition_type;               /* 0x04 */
    message_delta_processor_mode                    mode;                          /* 0x08 */
    void                                           *buffer;                        /* 0x0C */
    int                                             buffer_size_in_bits;           /* 0x10 */
    int                                             bits_written;                  /* 0x14 */
    int                                             bits_left;                     /* 0x18 */
    bitstream_t                                     iteration_independent_stream;  /* 0x1C */
    int                                             iterations_written;            /* 0x38 */
    int                                             iteration_start_bit;           /* 0x3C */
    int                                             iteration_header_bits_written; /* 0x40 */
    int                                             iteration_body_bits_written;   /* 0x44 */
    bitstream_t                                     iteration_field_map_stream;    /* 0x48 */
    bitstream_t                                     iteration_data_stream;         /* 0x64 */
    int                                             message_mode_bits;             /* 0x80 */
    int                                             message_id_bits;               /* 0x84 */
    int                                             protocol_bits;                 /* 0x88 */
    int                                             iteration_count_bits;          /* 0x8C */
    int                                             padding_bits;                  /* 0x90 */
} iterated_message;
