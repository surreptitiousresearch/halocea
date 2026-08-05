/* default_sound_environment @ 0x82128548 (.rdata, 72 bytes == sizeof(sound_environment)) — the
 * neutral EAX reverb parameter set. scenario_initialize_for_new_map seeds the scenario globals'
 * sound_environment_interpolator from it, and scenario_get_sound_environment returns it when no
 * environment applies.
 * Initializer reconstructed from the binary (big-endian words):
 *   +0x00 0x00000000 -> pad1                  = 0
 *   +0x04 0x00000000 -> priority = 0, pad2    = 0
 *   +0x08 0x00000000 -> room_intensity        = 0.0
 *   +0x0C 0x00000000 -> room_intensity_hf     = 0.0
 *   +0x10 0x00000000 -> room_rolloff_factor   = 0.0
 *   +0x14 0x3F800000 -> decay_time            = 1.0
 *   +0x18 0x3F000000 -> decay_hf_ratio        = 0.5
 *   +0x1C 0x00000000 -> reflections_intensity = 0.0
 *   +0x20 0x3CA3D70A -> reflections_delay     = 0.02
 *   +0x24 0x00000000 -> reverb_intensity      = 0.0
 *   +0x28 0x3D23D70A -> reverb_delay          = 0.04
 *   +0x2C 0x3F800000 -> diffusion             = 1.0
 *   +0x30 0x3F800000 -> density               = 1.0
 *   +0x34 0x459C4000 -> hf_reference          = 5000.0
 *   +0x38..+0x44 0x00000000 x4 -> unused[4]   = { 0, 0, 0, 0 }
 *
 * DEVIATION: the object lives in .rdata (read-only), so the original declaration was almost
 * certainly const; it is spelled non-const here to match the extern already carried by
 * src/headers/sound_environment.h and by both consumers.
 */
#include "../headers/sound_environment.h"

sound_environment default_sound_environment =
{
    0,        /* pad1                  */
    0,        /* priority              */
    0,        /* pad2                  */
    0.0f,     /* room_intensity        */
    0.0f,     /* room_intensity_hf     */
    0.0f,     /* room_rolloff_factor   */
    1.0f,     /* decay_time            */
    0.5f,     /* decay_hf_ratio        */
    0.0f,     /* reflections_intensity */
    0.02f,    /* reflections_delay     */
    0.0f,     /* reverb_intensity      */
    0.04f,    /* reverb_delay          */
    1.0f,     /* diffusion             */
    1.0f,     /* density               */
    5000.0f,  /* hf_reference          */
    { 0, 0, 0, 0 },  /* unused          */
};
