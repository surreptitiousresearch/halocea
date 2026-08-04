/* platform_definitions @ 0x84177D38 — .data data-init (platform_sound_manager_definition *[2]).
 * Reloc order: [0] = &platform_sound_dsound (0x84175EE8), [1] = NULL. */
#include "../headers/platform_sound_manager_definition.h"
extern platform_sound_manager_definition platform_sound_dsound; /* @ 0x84175EE8 (defined elsewhere) */
platform_sound_manager_definition *platform_definitions[2] = {
    &platform_sound_dsound,
    0,
};
