#pragma once
/* compressed_quaternion_8byte — a quaternion stored as four signed 16-bit components (i,j,k,w), each the float
 * component scaled by 32767. */

typedef struct compressed_quaternion_8byte
{
    __int16 i;   /* 0x00 */
    __int16 j;   /* 0x02 */
    __int16 k;   /* 0x04 */
    __int16 w;   /* 0x06 */
} compressed_quaternion_8byte;
