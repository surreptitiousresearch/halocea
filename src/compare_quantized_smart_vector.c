#include "headers/quantized_smart_vector.h"

BOOL compare_quantized_smart_vector(const quantized_smart_vector *const in1, const quantized_smart_vector *const in2)
{
    if (in1->x != in2->x)
        return 0;
    if (in1->y != in2->y)
        return 0;
    return in1->z == in2->z;
}
