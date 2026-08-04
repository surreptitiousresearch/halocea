#ifndef INPUT_DEVICE_DEFAULTS_H
#define INPUT_DEVICE_DEFAULTS_H

#include "tag_data.h"

/* input_device_defaults_device_type - DB-verified via types_enum_values
 * $62C6FF32E70ECF8DD4A17A0F34604E8C (values verbatim). */
enum input_device_defaults_device_type {
	_input_device_defaults_device_type_mouse_keyboard = 0,
	_input_device_defaults_device_type_joystick = 1,
	_input_device_defaults_device_type_full_profile = 2,
	NUMBER_OF_INPUT_DEVICE_DEFAULTS_DEVICE_TYPES = 3,
};

/* input_device_defaults - the "devc" tag definition. DB-verified layout (44 bytes). */
typedef struct input_device_defaults
{
	__int16  device_type;  /* 0x00 - input_device_defaults_device_type */
	__int16  flags;        /* 0x02 */
	tag_data device_id;    /* 0x04 */
	tag_data profile;      /* 0x18 - a player_profile blob for full_profile devices */
} input_device_defaults;   /* 44 bytes */

#endif /* INPUT_DEVICE_DEFAULTS_H */
