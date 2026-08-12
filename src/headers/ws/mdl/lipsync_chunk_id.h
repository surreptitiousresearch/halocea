#pragma once

/* Lip-sync spline (.spl) fio chunk ids.
 * Source: types_enum_values `lipsync::<unnamed_tag>` (DB-verified, members verbatim in DB order).
 * The DB scopes this unnamed enum to the `lipsync` namespace, so the enumerators are spelled
 * `lipsync::FIO_CNK_*` at use sites. Consumer: src/ws/mdl/mdlLIP_SYNCER__Start.cpp. */
namespace lipsync {
	enum {
		FIO_CNK_PHONEM = 0x1E0,      /* 480 */
		FIO_CNK_STATE = 0x1E1,       /* 481 */
		FIO_CNK_SAMPLE_RATE = 0x1E2, /* 482 */
	};
}

// lipsync chunk ids: DB-verified via types_enum_values `lipsync::<unnamed_tag>`
