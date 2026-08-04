/* default_render_model_effect @ 0x844A58E0 — default render_model_effect used by render_model when the
 * caller passes no model_effect. data-bss zero-fill, DB type render_model_effect (40 bytes).
 * P4 sweep flagged addi at +0x28/+0x48: +0x28 (== struct size) reaches the adjacent
 * default_render_model_region_permutation_indices global via a shared base register (sibling-block access), not
 * overflow. Type render_model_effect is correct. */
#include "../headers/render_model_effect.h"

render_model_effect default_render_model_effect;
