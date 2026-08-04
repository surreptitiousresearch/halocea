#ifndef GLOW_EFFECT_BOUNDARY_TYPE_H
#define GLOW_EFFECT_BOUNDARY_TYPE_H

/* glow_effect_boundary_type — glow_definition.boundary_effect: how a glow particle's
 * lifetime cursor (t) behaves at the [0, total_time] boundary.
 * DB anonymous enum _2E4E5554BE41BCC2F1DF1E8BB7A144AD (types_enum_values). */
enum glow_effect_boundary_type
{
	_glow_effect_boundary_type_bounce      = 0,   /* ping-pong: reflect t and flip traversal direction */
	_glow_effect_boundary_type_wrap        = 1,   /* modular wrap of t by total_time */
	NUMBER_OF_GLOW_EFFECT_BOUNDARY_TYPES   = 2,
};

#endif /* GLOW_EFFECT_BOUNDARY_TYPE_H */
