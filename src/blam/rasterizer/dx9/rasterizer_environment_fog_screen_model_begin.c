/* rasterizer_environment_fog_screen_model_begin @0x8378FFB8 — always-false stub (screen fog models never
 * gate the caller's setup). */

#include <stdint.h>
#include "headers/rasterizer_model_begin_parameters.h"

uint8_t rasterizer_environment_fog_screen_model_begin(const rasterizer_model_begin_parameters *parameters)
{
    (void)parameters;
    return 0;
}
