#pragma once

typedef struct input_abstraction_input_button
{
    __int16 device_type;     /* 0x00 */
    __int16 device_number;   /* 0x02 */
    __int16 button_type;     /* 0x04 */
    __int16 button_number;   /* 0x06 */
    int parameter;           /* 0x08 */
} input_abstraction_input_button;
