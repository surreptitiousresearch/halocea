/* Params are Blam `real` (float); the Hex-Rays PPC decompiler defaulted them to `double` and wrapped
 * every use in a narrowing cast — the binary does the arithmetic entirely in single precision
 * (fsubs/fmuls/fmadds, no frsp). Reconstructed as:
 *   result = (scalar1 - scalar0) * s + (scalar2 - scalar0) * t + scalar0. */
float shade_scalar(float scalar0, float scalar1, float scalar2, float s, float t)
{
    return (scalar1 - scalar0) * s + (scalar2 - scalar0) * t + scalar0;
}
