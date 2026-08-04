#ifndef HS_THREAD_FLAGS_H
#define HS_THREAD_FLAGS_H

/* hs_thread_flags — bit indices for hs_thread.flags (HaloScript thread state; thread->flags in hs_*.c).
 * Source: reference enum _C55B2A50A54A8B73F58237B7F0758180 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum hs_thread_flags {
	_hs_thread_in_function_call_bit = 0x0,
	_hs_thread_latent_sleep_bit = 0x1,
	NUMBER_OF_HS_THREAD_FLAGS = 0x2,
};

#endif /* HS_THREAD_FLAGS_H */

// hs_thread_flags: DB-verified via types_enum_values _C55B2A50A54A8B73F58237B7F0758180 (anonymous compiled enum, names sans leading _)
