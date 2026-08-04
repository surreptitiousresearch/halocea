#pragma once
/* GUESS: enum recovered by reconciliation, no ground-truth source (DB types the selector as a plain
 * __int16; no DB enum / headers_ref hash exists). Rationale: these are the canonical Halo tag-definition
 * "periodic function" values, and periodic_function_build_table's per-case math matches each name exactly:
 *   0  one                          -> value = 1
 *   1  zero                         -> value = 0
 *   2  cosine                       -> cos(phase)
 *   3  cosine_variable_period       -> cos(variable_phase)
 *   4  diagonal_wave                -> triangle wave over phase
 *   5  diagonal_wave_variable_period-> triangle wave over variable_phase
 *   6  slide                        -> sawtooth fmod(phase,1)   (emitted un-rescaled, mask 0xC0)
 *   7  slide_variable_period        -> sawtooth fmod(var_phase,1) (emitted un-rescaled)
 *   8  noise                        -> real_seed_random()
 *   9  jitter / 10 wander           -> shared multi-harmonic sum
 *   11 spark                        -> f*f
 * Names are the well-known Halo enumerators; not a cited symbol. */

/* layout bound to DB enum: types_enum_values _C9997616ADE70A091450A9989C0C465C */
enum periodic_function
{
	_periodic_function_one = 0x0,
	_periodic_function_zero = 0x1,
	_periodic_function_cosine = 0x2,
	_periodic_function_cosine_with_random_period = 0x3,
	_periodic_function_diagonal_wave = 0x4,
	_periodic_function_diagonal_wave_with_random_period = 0x5,
	_periodic_function_slide = 0x6,
	_periodic_function_slide_with_random_period = 0x7,
	_periodic_function_noise = 0x8,
	_periodic_function_jitter = 0x9,
	_periodic_function_wander = 0xA,
	_periodic_function_spark = 0xB,
	NUMBER_OF_PERIODIC_FUNCTIONS = 0xC,
};
