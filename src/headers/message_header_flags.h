#ifndef MESSAGE_HEADER_FLAGS_H
#define MESSAGE_HEADER_FLAGS_H

/* message_header_flags — message_header flags.
 * DB-verified: types_enum_values _1E9AB273DDF22CFDC4AAF6CD55FDFCBE carries these named
 * values verbatim. Bit-index members use as (1u << _bit); *_MASK / *_MAXIMUM
 * / large members are composite masks or constants, used as-is. */
enum message_header_flags {
	_message_flag_encrypted_bit = 0,
	_message_flag_new_encryption_key_bit = 1,
	_message_type_bit0 = 2,
	_message_type_bit1 = 3,
	_message_size_bit0 = 4,
	_message_size_bit1 = 5,
	_message_size_bit2 = 6,
	_message_size_bit3 = 7,
	_message_size_bit4 = 8,
	_message_size_bit5 = 9,
	_message_size_bit6 = 10,
	_message_size_bit7 = 11,
	_message_size_bit8 = 12,
	_message_size_bit9 = 13,
	_message_size_bit10 = 14,
	_message_size_bit11 = 15,
	NUMBER_OF_MESSAGE_HEADER_BITS = 16,
	_message_type_unknown = 0,
	_message_type_error = 1,
	_message_type_data = 2,
	_message_type_packet = 3,
	NUMBER_OF_MESSAGE_TYPES = 4,
	MESSAGE_FLAG_BITS_MASK = 3,
	MESSAGE_TYPE_FLAG_BITS_MASK = 12,
	MESSAGE_SIZE_FLAG_BITS_MASK = 65520,
	_byte_order_host = 0,
	_byte_order_network = 1,
	MAXIMUM_ERROR_STRING_LENGTH = 128,
	MAXIMUM_MESSAGE_TYPE = 3,
	MAXIMUM_MESSAGE_SIZE = 4095,
	MAXIMUM_MESSAGE_SIZE_BITS = 32760,
};

#endif /* MESSAGE_HEADER_FLAGS_H */

// message_header_flags: DB-verified via types_enum_values _1E9AB273DDF22CFDC4AAF6CD55FDFCBE
