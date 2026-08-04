#ifndef DEVICE_DATUM_FLAGS_H
#define DEVICE_DATUM_FLAGS_H

/* device_datum_flags — bit indices for device_datum.device.flags (device->device.flags).
 * Source: reference enum _C25C73D2E2CFE944F6EBA1C583A9153F (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum device_datum_flags {
	_device_position_reversed_bit = 0x0,
	_device_not_usable_bit = 0x1,
	_device_animation_changed_bit = 0x2,
	NUMBER_OF_DEVICE_FLAGS = 0x3,
};

#endif /* DEVICE_DATUM_FLAGS_H */

// device_datum_flags: DB-verified via types_enum_values _C25C73D2E2CFE944F6EBA1C583A9153F (anonymous compiled enum, names sans leading _)
