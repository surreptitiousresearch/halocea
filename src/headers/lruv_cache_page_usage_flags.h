#ifndef LRUV_CACHE_PAGE_USAGE_FLAGS_H
#define LRUV_CACHE_PAGE_USAGE_FLAGS_H

/* lruv_cache_page_usage_flags - bit indices for the lruv_cache_page_usage.flags word.
 * DB-verified: types_enum_values _D4CB22D88EBF2BC3CA7809627F1AC555 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum lruv_cache_page_usage_flags {
	_lruv_cache_page_usage_allocated_bit = 0,
	_lruv_cache_page_usage_used_this_frame_bit = 1,
	_lruv_cache_page_usage_old_bit = 2,
	_lruv_cache_page_usage_locked_bit = 3,
	NUMBER_OF_LRUV_CACHE_PAGE_USAGE_FLAGS = 4,
};

#endif /* LRUV_CACHE_PAGE_USAGE_FLAGS_H */

// lruv_cache_page_usage_flags: DB-verified via types_enum_values _D4CB22D88EBF2BC3CA7809627F1AC555
