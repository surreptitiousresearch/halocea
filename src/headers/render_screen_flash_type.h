#ifndef RENDER_SCREEN_FLASH_TYPE_H
#define RENDER_SCREEN_FLASH_TYPE_H

/* render_screen_flash_type — the renderer-side screen-flash blend mode stored in render_screen_flash.type.
 * DB-verified: types_enum_values _3663497A9FDAFC7EF202160A856B97C7 carries these named values verbatim. */
enum render_screen_flash_type {
	_render_screen_flash_type_none   = 0,
	_render_screen_flash_type_lighten = 1,
	_render_screen_flash_type_darken = 2,
	_render_screen_flash_type_max    = 3,
	_render_screen_flash_type_min    = 4,
	_render_screen_flash_type_invert = 5,
	_render_screen_flash_type_tint   = 6,
	NUMBER_OF_RENDER_SCREEN_FLASH_TYPES = 7,
};

/* screen_flash_type @ 0x84175BE4 — standalone int holding a default render_screen_flash_type value. */
#ifdef __cplusplus
extern "C" {
#endif

extern int screen_flash_type;

#ifdef __cplusplus
}
#endif

#endif /* RENDER_SCREEN_FLASH_TYPE_H */
