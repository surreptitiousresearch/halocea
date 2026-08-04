#ifndef CONTRAIL_RENDER_TYPE_H
#define CONTRAIL_RENDER_TYPE_H

/* contrail_definition render_type + the render-pass flag masks used to select which contrail
 * render types are drawn in a given pass.
 * DB-verified: types_enum_values _A95FA4AC4B945014A09D31EAD744EEC1 carries these names verbatim. */

enum contrail_render_type
{
	_contrail_render_type_vertical = 0,
	_contrail_render_type_horizontal = 1,
	_contrail_render_type_media = 2,
	_contrail_render_type_ground = 3,
	_contrail_render_type_viewer = 4,
	_contrail_render_type_linked = 5,
	NUMBER_OF_CONTRAIL_DEFINITION_RENDER_TYPES = 6,

	/* render-pass selection masks (bit N = 1 << _contrail_render_type_N) */
	CONTRAIL_RENDER_TYPE_GROUND_FLAGS = 8,      /* 1 << _contrail_render_type_ground */
	CONTRAIL_RENDER_TYPE_MEDIA_FLAGS = 4,       /* 1 << _contrail_render_type_media */
	CONTRAIL_RENDER_TYPE_IMMEDIATE_FLAGS = 12,  /* ground | media */
	CONTRAIL_RENDER_TYPE_NORMAL_FLAGS = -13,    /* ~(ground | media) */
};

#endif /* CONTRAIL_RENDER_TYPE_H */
