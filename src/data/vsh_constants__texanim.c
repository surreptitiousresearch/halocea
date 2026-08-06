/* vsh_constants__texanim @ 0x82128E78 (.rdata, 128 bytes)
 * DB applied_types: const float vsh_constants__texanim[32];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 0x3F800000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0xBC000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x3F800000
 *   +0x0018 = 0x00000000
 *   +0x001C = 0xBC000000
 *   +0x0020 = 0x3F800000
 *   +0x0024 = 0x00000000
 *   +0x0028 = 0x00000000
 *   +0x002C = 0x3C000000
 *   +0x0030 = 0x00000000
 *   +0x0034 = 0x3F800000
 *   +0x0038 = 0x00000000
 *   +0x003C = 0x3C000000
 *   +0x0040 = 0x3F800000
 *   +0x0044 = 0x00000000
 *   +0x0048 = 0x00000000
 *   +0x004C = 0xBC000000
 *   +0x0050 = 0x00000000
 *   +0x0054 = 0x3F800000
 *   +0x0058 = 0x00000000
 *   +0x005C = 0x3C000000
 *   +0x0060 = 0x3F800000
 *   +0x0064 = 0x00000000
 *   +0x0068 = 0x00000000
 *   +0x006C = 0x3C000000
 *   +0x0070 = 0x00000000
 *   +0x0074 = 0x3F800000
 *   +0x0078 = 0x00000000
 *   +0x007C = 0xBC000000
 * .rdata, 128 bytes = 32 floats = 8 vertex-shader float4
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
 * DEVIATION: src/rasterizer_sun_glow_convolve.c declares this `extern float
 * vsh_constants__texanim[]` with no const; the object is in .rdata and the DB applied type is
 * const. Defined const.
 */
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
