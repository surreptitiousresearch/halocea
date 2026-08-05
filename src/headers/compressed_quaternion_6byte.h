#pragma once
#include <stdint.h>
/* compressed_quaternion_6byte — a unit quaternion packed into 6 bytes (three 16-bit words), decoded by
 * quaternion_decompress_6byte. The component bits straddle the word boundaries as the field names indicate. */

typedef struct compressed_quaternion_6byte
{
    uint16_t iiij;   /* 0x00 */
    uint16_t jjkk;   /* 0x02 */
    uint16_t kwww;   /* 0x04 */
} compressed_quaternion_6byte;
