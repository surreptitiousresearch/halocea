/* vsh_constants__screenproj @ 0x82125BD8 (.rdata, 80 bytes)
 * DB applied_types: const float vsh_constants__screenproj[5][4];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 0x3F800000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x3F800000
 *   +0x0018 = 0x00000000
 *   +0x001C = 0x00000000
 *   +0x0020 = 0x00000000
 *   +0x0024 = 0x00000000
 *   +0x0028 = 0x3F800000
 *   +0x002C = 0x00000000
 *   +0x0030 = 0x00000000
 *   +0x0034 = 0x00000000
 *   +0x0038 = 0x00000000
 *   +0x003C = 0x3F800000
 *   +0x0040 = 0x3F800000
 *   +0x0044 = 0x3F800000
 *   +0x0048 = 0x00000000
 *   +0x004C = 0x3F800000
 * .rdata, 80 bytes = 20 floats = 5 vertex-shader float4
 * constant registers. _rasterizer_hud_motion_sensor_blip_end uploads it with
 * D3DDevice_SetVertexShaderConstantFN(device, 0xD, vsh_constants__screenproj, 5, 3ULL << 59), so
 * the Vector4fCount of 5 independently fixes the vector count.
 * Big-endian words -> IEEE-754, grouped four to a register:
 *   c13 +0x0000 {1, 0, 0, 0}
 *   c14 +0x0010 {0, 1, 0, 0}   rows 0-3 are the 4x4 identity: vertices are already in
 *   c15 +0x0020 {0, 0, 1, 0}   normalized device coordinates for the sweep and ring passes
 *   c16 +0x0030 {0, 0, 0, 1}
 *   c17 +0x0040 {1, 1, 0, 1}   the trailing scale/bias register
 * The same five registers are re-uploaded from a viewport-derived matrix for the final on-screen
 * pass, which is why this constant block only has to carry the identity case.
 * DEVIATION: the DB applied type spells this `const float[5][4]`. Defined flat as `const float[20]`
 * to match the sole declaration (`extern const float vsh_constants__screenproj[]`, which decays to
 * const float * at the upload call) and the sibling vsh_constants__texanim, whose DB applied type
 * is flat `const float[32]`. Identical 80 bytes either way; the float4 grouping is in the layout
 * above and in the initializer's line breaks.
 */
const float vsh_constants__screenproj[20] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f
};
