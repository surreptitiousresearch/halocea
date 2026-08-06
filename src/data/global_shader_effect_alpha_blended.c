/* global_shader_effect_alpha_blended @ 0x84186D10 (.data, 180 bytes)
 * DB applied_types: shader_effect global_shader_effect_alpha_blended;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 shader                     = <40 bytes>
 *     +0x24 = 0x00010000
 *   +0x28 effect                     = <140 bytes>
 *     +0x4C = 0x6269746D
 *     +0x50 = 0x8200155A -> "" (empty string at target)
 *     +0x58 = 0xFFFFFFFF
 * 180 bytes
 *    (data-init; ledger mislabeled data-bss).
 *    Built-in "alpha blended" shader_effect default. Only a handful of fields are
 *    non-zero in the binary; reconstructed from the raw image (big-endian).
 *    DEVIATION: the prose here used to call secondary_map.name "the absolute image address
 *    baked into the .data image (points at non-string image data)". 0x8200155A is the shared
 *    EMPTY STRING every unnamed tag_reference points at — IDA's `strings` table indexes
 *    neither the empty string nor 1-character ones, which is why it read as raw image data.
 *    Reproduced verbatim for byte fidelity either way, but it is a string.
 *    NOT MACHINE-VERIFIED (as of 2026-08-06): `--verify` cannot align this initializer,
 *    because .sweep/data_layouts.json holds one level of members only — shader_effect is
 *    there, `_shader` and `_shader_effect` are not, so `.shader = { .type = 1 }` descends
 *    into an opaque leaf. `--dump` now closes the type graph transitively; this TU checks
 *    itself the next time the image is re-dumped. Values above were confirmed by hand
 *    against the image words listed in this block.
 */
#include "../headers/shader_effect.h"

shader_effect global_shader_effect_alpha_blended = {
    .shader = {
        .type = 1,                       /* offset 36 */
    },
    .effect = {
        .secondary_map = {               /* effect offset 36 (abs 76) */
            .group_tag   = 0x6269746Du,  /* 'bitm' */
            .name        = (char *)0x8200155A,
            .name_length = 0,
            .index       = -1,           /* 0xFFFFFFFF */
        },
    },
};
