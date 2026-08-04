#include "headers/path_input.h"

void path_input_set_search_bounds(path_input *input, float maximum_distance)
{
    input->search_maximum_distance = maximum_distance;
    input->search_bounded = 1;
}
