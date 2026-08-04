#pragma once
/* _message_definition — describes one message-delta message type: its serialized size accounting and the
 * header/body field reference sets used to encode/decode it. Indexed by
 * message_delta_processor_message_definition_type via message_delta_global_message_list. */

#include "message_definition_field_reference_set.h"

/* the message_delta definition-type id is used as an array index; treat as an int */
typedef int message_delta_processor_message_definition_type;

typedef struct _message_definition
{
    const message_delta_processor_message_definition_type definition_type;   /* 0x00 */
    int                                     header_size_in_bits;             /* 0x04 */
    int                                     iteration_size_in_bits;          /* 0x08 */
    int                                     iteration_independent_size_in_bits; /* 0x0C */
    int                                     maximum_total_size;              /* 0x10 */
    int                                     max_iterations;                  /* 0x14 */
    unsigned __int8                         initialized;                     /* 0x18 */
    unsigned char _pad0[3]; /* db-verified padding */
    _message_definition_field_reference_set *const header_fields;            /* 0x1C */
    _message_definition_field_reference_set body_fields;                     /* 0x20 (8 bytes) */
} _message_definition;
