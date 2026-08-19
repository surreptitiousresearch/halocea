/* shader_stage_cfg_eq @ 0x836A1418 — compare two texture-stage configs for
 * equality (color_func, alpha_func, a_replicate all match). */

#include "headers/shader_stage_cfg.h"

int shader_stage_cfg_eq(const shader_stage_cfg *s1, const shader_stage_cfg *s2)
{
    return s1->a_replicate == s2->a_replicate
        && s1->color_func == s2->color_func
        && s1->alpha_func == s2->alpha_func;
}
