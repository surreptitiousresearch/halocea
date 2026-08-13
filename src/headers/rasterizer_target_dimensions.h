#pragma once

/* rasterizer_target_dimensions — the render-target width/height/size constants of
 * global_render_targets (member-for-member against the target table, rows indexed by
 * enum rasterizer_target $A4EFD520). Pinned by the round-2 splitscreen hunt.
 * Source: types_enum_values $3CFA98D8DDFFD7EF7DAB881AC8889635 (DB-verified, members verbatim in DB order). */
enum rasterizer_target_dimensions {
	RASTERIZER_TARGET_RENDER_PRIMARY_WIDTH = 640,
	RASTERIZER_TARGET_RENDER_PRIMARY_HEIGHT = 480,
	RASTERIZER_TARGET_RENDER_SECONDARY_WIDTH = 320,
	RASTERIZER_TARGET_RENDER_SECONDARY_HEIGHT = 240,
	RASTERIZER_TARGET_SHADOW_PRIMARY_SIZE = 128,
	RASTERIZER_TARGET_SHADOW_SECONDARY_SIZE = 128,
	RASTERIZER_TARGET_SUN_GLOW_SIZE = 64,
	RASTERIZER_TARGET_WATER_SIZE = 128,
	RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS = 4,
	RASTERIZER_TARGET_MOTION_SENSOR_SIZE = 64,
};

// rasterizer_target_dimensions: DB-verified via types_enum_values $3CFA98D8DDFFD7EF7DAB881AC8889635
