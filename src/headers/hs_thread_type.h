#ifndef HS_THREAD_TYPE_H
#define HS_THREAD_TYPE_H

/* hs_thread_type — what an hs_thread executes (hs_thread.type).
 * Source: reference enum _D35A29511818E1E5800EBDC6CB0F4AFD (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum hs_thread_type {
	hs_thread_type_script = 0,
	hs_thread_type_global_initialize = 1,
	hs_thread_type_runtime_evaluate = 2,
	number_of_hs_thread_types = 3
} ;

#endif /* HS_THREAD_TYPE_H */

// hs_thread_type: DB-verified via types_enum_values _D35A29511818E1E5800EBDC6CB0F4AFD (anonymous compiled enum, names sans leading _)
