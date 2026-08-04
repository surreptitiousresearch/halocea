#pragma once
/* message_delta_encoding_class — selects the field-encoding precision used by the message-delta system.
 * LAN games use wider (more precise) encodings; internet games use the delta/compressed paths. Value from the
 * gMessageEncodingClass comparison in default_locality_reference_position_encoder (== 1 for internet). */

/* layout bound to DB enum: types_enum_values message_delta_processor_encoding_class */
typedef enum message_delta_processor_encoding_class
{
	_message_delta_encoding_class_lan = 0x0,
	_message_delta_encoding_class_internet = 0x1,
} message_delta_processor_encoding_class;

typedef message_delta_processor_encoding_class message_delta_encoding_class;
