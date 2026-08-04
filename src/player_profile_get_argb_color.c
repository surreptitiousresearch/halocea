#include "headers/real_argb_color.h"

extern real_rgb_color *player_profile_get_rgb_color(real_rgb_color *result, int index);

real_argb_color * player_profile_get_argb_color(real_argb_color *result, int index)
{
    real_rgb_color rgb_color;
    player_profile_get_rgb_color(&rgb_color, index);

    result->n[0] = 1.0f;
    result->n[1] = rgb_color.n[0];
    result->n[2] = rgb_color.n[1];
    result->n[3] = rgb_color.n[2];
    return result;
}
