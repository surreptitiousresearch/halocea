#pragma once
/* transition_function — tag-definition transition-function selector values. DB-verified: the anonymous
 * compiled enum _E439FB19D4C03195A0D6D73AE68D26FD carries these named values verbatim (see footer).
 * transition_function_build_table's per-case math matches each name:
 *   0  linear     -> x
 *   1  early      -> pow(x, 0.5)   (rises quickly)
 *   2  very_early -> pow(x, 0.25)  (rises even faster)
 *   3  late       -> pow(x, 2)     (rises slowly then fast)
 *   4  very_late  -> pow(x, 4)
 *   5  cosine     -> (sin(x*pi - pi/2) + 1) / 2   (smoothstep)
 * Names are the well-known Halo enumerators; not a cited symbol. */

enum transition_function
{
    _transition_function_linear     = 0,
    _transition_function_early      = 1,
    _transition_function_very_early = 2,
    _transition_function_late       = 3,
    _transition_function_very_late  = 4,
    _transition_function_cosine     = 5,
    NUMBER_OF_TRANSITION_FUNCTIONS  = 6,
};

// transition_function: DB-verified via types_enum_values _E439FB19D4C03195A0D6D73AE68D26FD (anonymous compiled enum, names sans leading _)
