/* default_function_values @ 0x844A58D0 - data-bss zero-fill; DB float[4] (16). ledger size 136 overcount.
 * P4 sweep flagged addi at +0x10/+0x38/+0x58: these reach the ADJACENT default_render_model_* globals
 * (+0x10=default_render_model_effect, +0x38=default_render_model_region_permutation_indices,
 * +0x58=default_render_model_change_colors) via a shared base register.
 * Sibling-block access, not overflow of this array. Type float[4] is correct. */

float default_function_values[4];
