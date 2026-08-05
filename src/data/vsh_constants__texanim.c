/* vsh_constants__texanim @0x82128E78 — .rdata, 128 bytes = 32 floats = 8 vertex-shader float4
 * constant registers; DB applied type `const float[32]`. rasterizer_sun_glow_convolve uploads it
 * with D3DDevice_SetVertexShaderConstantFN(device, 0xD, vsh_constants__texanim, 8, 7ULL << 58), so
 * the Vector4fCount of 8 independently fixes the vector count.
 * Big-endian words -> IEEE-754, grouped four to a register. The registers pair up into four 2-row
 * affine texture-coordinate transforms (u' = u + du, v' = v + dv), one per sampler stage; 0.0078125
 * is 1/128, so each stage samples a half-texel diagonal tap of the 4-tap convolution kernel:
 *   stage 0  c13 {1,0,0,-1/128}  c14 {0,1,0,-1/128}   (-u, -v)
 *   stage 1  c15 {1,0,0,+1/128}  c16 {0,1,0,+1/128}   (+u, +v)
 *   stage 2  c17 {1,0,0,-1/128}  c18 {0,1,0,+1/128}   (-u, +v)
 *   stage 3  c19 {1,0,0,+1/128}  c20 {0,1,0,-1/128}   (+u, -v)
 * That matches the consumer binding the source render target on all four sampler stages before
 * each additive blur pass.
 *
 * DEVIATION: src/rasterizer_sun_glow_convolve.c declares this `extern float
 * vsh_constants__texanim[]` with no const; the object is in .rdata and the DB applied type is
 * const. Defined const. */

const float vsh_constants__texanim[32] =
{
    1.0f, 0.0f, 0.0f, -0.0078125f,
    0.0f, 1.0f, 0.0f, -0.0078125f,
    1.0f, 0.0f, 0.0f,  0.0078125f,
    0.0f, 1.0f, 0.0f,  0.0078125f,
    1.0f, 0.0f, 0.0f, -0.0078125f,
    0.0f, 1.0f, 0.0f,  0.0078125f,
    1.0f, 0.0f, 0.0f,  0.0078125f,
    0.0f, 1.0f, 0.0f, -0.0078125f
};
