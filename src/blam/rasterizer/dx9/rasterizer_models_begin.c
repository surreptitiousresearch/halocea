/* rasterizer_models_begin @0x836A6120 */
/* rasterizer_models_begin 0x836A6120 (thunk) — tail-call thunk to the underscore rasterizer implementation. */

#include <stdint.h>

extern void _rasterizer_models_begin(uint8_t sky);

void rasterizer_models_begin(uint8_t sky)
{
    _rasterizer_models_begin(sky);
}
