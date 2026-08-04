#ifndef BITMAP_TYPE_H
#define BITMAP_TYPE_H

/* bitmap_type — a bitmap_data's dimensionality/class (bitmap_data.type).
 * Source: reference enum _AC1A86CCF16C657D88260FEECA86B352 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. NOTE: bitmap_group.type is a DIFFERENT enum (not this one). */
enum bitmap_type {
	bitmap_type_2d = 0,
	bitmap_type_3d = 1,
	bitmap_type_cube_map = 2,
	bitmap_type_white = 3,
	number_of_bitmap_types = 4,
};

#endif /* BITMAP_TYPE_H */

// bitmap_type: DB-verified via types_enum_values _AC1A86CCF16C657D88260FEECA86B352 (anonymous compiled enum, names sans leading _)
