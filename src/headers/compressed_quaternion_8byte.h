#pragma once
#include <stdint.h>
/* compressed_quaternion_8byte — a quaternion stored as four signed 16-bit components (i,j,k,w), each the float
 * component scaled by 32767. */

typedef struct compressed_quaternion_8byte
{
    int16_t i;   /* 0x00 */
    int16_t j;   /* 0x02 */
    int16_t k;   /* 0x04 */
    int16_t w;   /* 0x06 */
} compressed_quaternion_8byte;
