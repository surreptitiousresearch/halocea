/* choose_random_array_element @0x83800378 — weighted random pick among the unused (not set in
 * `used_bit_vector`) elements of a generic array: each element's weight is a float at `weight_field_offset`
 * within its `element_size`-byte record. Sums the weights of unused elements, rolls a random value in
 * [0, total_weight), then walks the unused elements again accumulating weight until the roll is covered.
 * Returns the chosen element's index, or -1 if there are no elements, no unused elements have positive
 * total weight, or (defensively) the walk runs off the end. */

#include <stdint.h>
#include "headers/bit_vector.h"

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

int choose_random_array_element(char *array, int16_t element_size, int16_t element_count,
        int16_t weight_field_offset, unsigned int *used_bit_vector)
{
    if ( element_count <= 0 )
        return -1;

    float total_weight = 0.0f;
    const float *weight = (const float *)&array[weight_field_offset];
    for ( int16_t i = 0; i < element_count; i++ )
    {
        if ( !BIT_VECTOR_TEST_FLAG(used_bit_vector, i) )
            total_weight += *weight;
        weight = (const float *)((const char *)weight + element_size);
    }

    if ( total_weight <= 0.0f )
        return -1;

    float roll = real_seed_random_range(get_global_random_seed_address(), 0.0f, total_weight);

    float accumulated = 0.0f;
    weight = (const float *)&array[weight_field_offset];
    int16_t index = 0;

    while ( 1 )
    {
        if ( !BIT_VECTOR_TEST_FLAG(used_bit_vector, index) )
        {
            accumulated += *weight;
            if ( roll <= accumulated )
                return index;
        }

        weight = (const float *)((const char *)weight + element_size);
        ++index;
        if ( index >= element_count )
            return -1;
    }
}
