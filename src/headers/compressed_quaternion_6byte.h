#pragma once
/* compressed_quaternion_6byte — a unit quaternion packed into 6 bytes (three 16-bit words), decoded by
 * quaternion_decompress_6byte. The component bits straddle the word boundaries as the field names indicate. */

typedef struct compressed_quaternion_6byte
{
    unsigned __int16 iiij;   /* 0x00 */
    unsigned __int16 jjkk;   /* 0x02 */
    unsigned __int16 kwww;   /* 0x04 */
} compressed_quaternion_6byte;
