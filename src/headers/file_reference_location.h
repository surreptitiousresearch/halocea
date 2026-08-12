#pragma once

/* file_reference_location — the storage-location id held in file_reference_info.location (byte +6 of a
 * file_reference) and passed as the `location` argument of file_reference_create @0x83767920,
 * file_location_get_full_path @0x837E6580 and file_location_set_volume @0x837678C8. The count is
 * corroborated by the volume-name table: applied_types gives `char file_location_volume_names[3][256]`,
 * one row per location. file_reference_create_from_path_absolute @0x83767D80 stamps location 2, and
 * references built for the raw device path store the locked -1 sentinel instead of a member.
 * Source: types_enum_values $E768BDE5E40507CA7E64B98CD5CB9F5D (DB-verified). */
enum file_reference_location {
	_file_reference_application_relative = 0,
	_file_reference_cd_relative = 1,
	_file_reference_absolute = 2,
	NUMBER_OF_FILE_REFERENCE_LOCATIONS = 3,
};

// file_reference_location: DB-verified via types_enum_values $E768BDE5E40507CA7E64B98CD5CB9F5D
