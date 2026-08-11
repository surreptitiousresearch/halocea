#pragma once
/* tag_groups — the tag_block / tag_data accessor family (Blam tag_files module).
 *
 * Before this header, no src/headers file declared ANY tag_block_* / tag_data_* function: all 25
 * caller TUs carried their own local extern (BACKLOG B12b-2), i.e. 25 independent spellings of five
 * symbols with nothing checking them against each other. Every signature below is the DB
 * applied_types decl in stdint spelling, EXCEPT the two the disassembly refutes; those carry a
 * DEVIATION note naming the caller evidence that decided them.
 *
 * The tag_files module is confirmed by the embedded source path
 * `D:\Projects\code\HCEX\sources\tag_files\files.c` (strings @ 0x8211E9D0).
 *
 * tag_block_delete_element (DB: `void __fastcall tag_block_delete_element(tag_block *, int)`
 * @ 0x836F7070, a bare `blr`) is deliberately NOT declared here: it has 0 direct callers in the
 * image and no definition TU in the corpus, so a declaration would be a link-closure claim with
 * nothing behind it. */

#include <stdint.h>
#include "tag_block.h"
#include "tag_data.h"

#ifdef __cplusplus
extern "C" {
#endif

/* tag_block_add_element @0x836F7068 — append an element and return its index, or -1 on failure
 * (always -1 in this cache-backed runtime build: the whole body is `li r3, -1; blr`).
 * DEVIATION: the DB decl spells the return `int`; the binary says int16_t. All 8 direct callers
 * consume r3 and 6 narrow it with `extsh` first — decisively, hs_scenario_merge @0x8372EBB4 does
 * `extsh r11, r3` and only then `cmpwi cr6, r11, -1`, so the -1 sentinel is compared after
 * sign-extension FROM A HALFWORD. A 32-bit return would need no extsh to compare against -1. */
extern int16_t tag_block_add_element(tag_block *block);

/* tag_block_resize @0x836F7058 — no-op in the cache (release) build (tag blocks are fixed at
 * cache-build time); always returns 0 = failure. uint8_t is the stdint spelling of the DB's
 * `unsigned __int8`, and is what the callers read: every one of the 7 (of 18) direct callers that
 * consumes r3 does it with `clrlwi rX, r3, 24` — hs_scenario_postprocess @0x83730024,
 * leaf_map_initialize_from_bsp @0x8381C3B0 et al. — a zero-extend of the low byte, the Blam bool. */
extern uint8_t tag_block_resize(tag_block *block, int element_count);

/* tag_block_get_element_with_size @0x83700B20 — address of element `index` given the element
 * stride: `block->address + index * element_size`. Three arguments, all read by the callee
 * (r3 @0x83700B20, r4 and r5 @0x83700B24 `mullw r10, r4, r5`). */
extern void *tag_block_get_element_with_size(const tag_block *block, int index, int element_size);

/* tag_data_resize @0x836F7060 — no-op in the cache (release) build; always returns 0 = failure.
 * DEVIATION: the DB decl spells the return `int`; the binary says an unsigned byte. 6 of the 10
 * direct callers consume r3, and 5 of those do it as `clrlwi rX, r3, 24` followed by `cmplwi rX, 0`
 * (hs_scenario_merge @0x8372EBF8, hs_compile_finish @0x8377A044/@0x8377A1FC, …) — the identical
 * Blam-bool shape to tag_block_resize, whose DB decl does say `unsigned __int8`. The 6th is a bare
 * `mr`, which discriminates nothing. Nothing in the image reads r3 as a full word. */
extern uint8_t tag_data_resize(tag_data *data, int size);

/* tag_data_get_pointer @0x83700B10 — `data->address + offset`. The release-build body reads only
 * r3 and r4; arity 3 is the DB decl and is confirmed at the call sites, which set all three
 * (hs_compile_source: `li r4, 0 # offset` @0x8372FC08, `lwz r5, 0x20(r11) # size` @0x8372FC14). */
extern void *tag_data_get_pointer(const tag_data *data, int offset, int size);

#ifdef __cplusplus
}
#endif
