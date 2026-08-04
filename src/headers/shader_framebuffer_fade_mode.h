#ifndef SHADER_FRAMEBUFFER_FADE_MODE_H
#define SHADER_FRAMEBUFFER_FADE_MODE_H

/* shader_framebuffer_fade_mode — how a transparent surface fades its framebuffer contribution with the
 * angle between the view direction and the surface normal. Used by build_sprite_compute_vertex_fade:
 * mode 1 returns cosine (→0 when perpendicular), mode 2 returns 1-cosine (→0 when parallel).
 * DB-verified: types_enum_values _9CC4C100111AA9322417810535860797 carries these named values verbatim. */
enum shader_framebuffer_fade_mode {
	_shader_framebuffer_fade_mode_none                  = 0,
	_shader_framebuffer_fade_mode_fade_when_perpendicular = 1,
	_shader_framebuffer_fade_mode_fade_when_parallel      = 2,
	NUMBER_OF_SHADER_FRAMEBUFFER_FADE_MODES             = 3,
};

#endif /* SHADER_FRAMEBUFFER_FADE_MODE_H */

// shader_framebuffer_fade_mode: DB-verified via types_enum_values _9CC4C100111AA9322417810535860797
