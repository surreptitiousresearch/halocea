/* linear_to_non_linear_alpha @0x83745FD8 — gamma-curves a linear [0,1] alpha value: pow(linear_alpha, 1.9).
 *
 * DEVIATION: the decompiler fabricates phantom `a2`/`a3` params (PPC long-double pow() ABI artifact, same
 * as director_process_variables.c). disasm_range (0x83745FD8-0x83745FF4) shows only f1 (linear_alpha) and a
 * loaded constant `__real_3ffe666660000000` (== float 1.9f promoted to double = 1.899999976158142...) feed
 * the single `pow` call; matches the DB's own 1-arg prototype. */

extern double pow(double base, double exponent);

float linear_to_non_linear_alpha(float linear_alpha)
{
    return (float)pow(linear_alpha, 1.899999976158142);
}
