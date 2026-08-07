#ifndef REFERENCE_INFO_FLAGS_H
#define REFERENCE_INFO_FLAGS_H

/* reference_info_flags — file_reference_info's flags word (0x004).
 * DB-verified: types_enum_values $19594DD35D63DC44799B0D605D50E24D (and the identical
 * $48BB4AD902270839DAD3C067498973A5) carry these named values verbatim. Values are BIT INDICES —
 * use as (1u << _bit).
 *
 * Reached 2026-08-07 by a route worth recording: migrating file_reference's raw-offset accessors
 * to their named view turned `*(unsigned short *)&file->data[4] & 1` into `file->info.flags & 1`,
 * which moved four sites INTO bitextract_strong and symptom_c for the first time. The detectors
 * had never been able to see them while they were spelled as a pointer pun — the same
 * spelling-drift blind spot recorded in the detector-spelling lesson, running the other way. */
enum reference_info_flags {
	_has_filename_bit = 0,
	NUMBER_OF_REFERENCE_INFO_FLAGS = 1,
};

#endif /* REFERENCE_INFO_FLAGS_H */
