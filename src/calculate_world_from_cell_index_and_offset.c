/* DEVIATION: decompiler double-return idiom; direct float. 0.0039215689 = 1/255.0 */
float calculate_world_from_cell_index_and_offset(float cell, float offset)
{
    return ((offset * (1.0f / 255.0f)) + cell) * 8.0f;
}
