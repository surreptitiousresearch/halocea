#pragma once
#include <stdint.h>

typedef struct input_abstraction_input_button
{
    int16_t device_type;     /* 0x00 */
    int16_t device_number;   /* 0x02 */
    int16_t button_type;     /* 0x04 */
    int16_t button_number;   /* 0x06 */
    int parameter;           /* 0x08 */
} input_abstraction_input_button;
