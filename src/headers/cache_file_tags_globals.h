#pragma once
/* cache_file_tags_globals — the tag-resolution view of `cache_file_globals`. Distinct from the
 * streaming cache_file_globals_s (a different object, cache_file_globals_0).
 *
 * DEVIATION: this used to define its OWN struct body, so the single object `cache_file_globals`
 * had two incompatible C types across the corpus (this one and cache_file_runtime_globals) —
 * latent because no TU had ever included both. The DB has exactly one type for the object,
 * anonymous struct $F4497E3014C013DA64EA5AA79E59B767 (types.size = 2060), and both bodies matched
 * it member-for-member. Consolidated to a typedef alias of the canonical definition so the two
 * externs are the same type; consumers keep using the name and members unchanged.
 *
 * cache_file_tags_header.h stays included here: consumers of this view dereference tags_header,
 * which the canonical header only forward-declares. */

#include "cache_file_runtime_globals.h"
#include "cache_file_tags_header.h"

typedef cache_file_runtime_globals cache_file_tags_globals;

extern cache_file_tags_globals cache_file_globals;
