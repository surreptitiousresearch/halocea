#pragma once
/* Accessors for the loaded-tag table: global_tag_instances is an array of 32-byte
 * cache_file_tag_instance entries, addressed by the low 16 bits of a tag handle. */

#include <stdint.h>
#include "cache_file_tag_instance.h"

/* TAG_GET(type, tag_index) — resolve a tag handle to the tag's loaded definition
 * block: global_tag_instances[(uint16_t)tag_index].base_address, typed.
 *
 * Why TAG_GET(type, index) and not Bungie's original spelling: the Blam API here
 * was `void *tag_get(tag group_tag, long tag_index)` — group_tag existed so debug
 * builds could assert the indexed slot's group matches the caller's expectation,
 * and every call site cast the void* result. In the release build we reconstruct
 * from, tag_get is inlined to exactly the indexing below and the group argument
 * is compiled away without a trace. Restoring the original signature would mean
 * inventing ~900 group-tag constants the binary cannot attest, so we bind the
 * result *type* (which each call site's field accesses do prove) and omit the
 * group (which nothing proves). Mechanically this mirrors DATUM_GET
 * (data_array.h): the handle's low-word extract — the decompiler's
 * ((32 * i) & 0x1FFFE0), identical to 32 * (uint16_t)i — lives here once instead
 * of being spelled as a *(type **) cast at every site, and struct indexing
 * replaces the hardcoded 32-byte stride so the expression stays correct when
 * pointer widths change (x64 port). */
#define TAG_GET(type, tag_index) \
    ((type *)global_tag_instances[(uint16_t)(tag_index)].base_address)

/* Same resolve through a cached local copy of the table pointer (the decompiler
 * frequently holds global_tag_instances in a register-local across a loop). */
#define TAG_GET_AT(table, type, tag_index) \
    ((type *)(table)[(uint16_t)(tag_index)].base_address)

/* TAG_INSTANCE(index) — address of element[index].base_address as char*, the raw
 * decompiler idiom (char*)&global_tag_instances->base_address + ((32*i) & 0x1FFFE0).
 * Legacy spelling kept only for the src/hcex C++ TUs that still use it; the .c
 * corpus uses TAG_GET. Do not use in new code. */
#define TAG_INSTANCE(index) \
    ((char *)&global_tag_instances->base_address + ((32 * (index)) & 0x1FFFE0))

/* Element (cache_file_tag_instance*) form: address of element[index] itself rather
 * than its base_address member. */
#define TAG_INSTANCE_ELEMENT(index) \
    ((cache_file_tag_instance *)((char *)global_tag_instances + ((32 * (index)) & 0x1FFFE0)))
