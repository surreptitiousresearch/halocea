#ifndef BITMAP_GROUP_TYPE_H
#define BITMAP_GROUP_TYPE_H

/* bitmap_group_type — a bitmap_group's tag type (bitmap_group.type).
 * DB anonymous enum _923B06D5BAC356CA02D45EE7691BAF7A (types_enum_values).
 * NOTE: distinct from bitmap_type (bitmap_data.type). */
enum bitmap_group_type
{
	_bitmap_group_type_2d_textures        = 0,
	_bitmap_group_type_3d_textures        = 1,
	_bitmap_group_type_cube_maps          = 2,
	_bitmap_group_type_sprites            = 3,
	_bitmap_group_type_interface_bitmaps  = 4,
	NUMBER_OF_BITMAP_GROUP_TYPES          = 5,
};

#endif /* BITMAP_GROUP_TYPE_H */
