/* RGBToColor @0x837E3FD0 */
#include <stdint.h>
#include "headers/s3tc_block.h"

void RGBToColor(const uint16_t *prgb, S3TC_COLOR *pcolor)
{
    unsigned int rgb565 = *prgb;
    S3TC_COLOR color;

    color.rgba[1] = ((rgb565 >> 9) & 3) | (4 * (rgb565 >> 5));
    color.rgba[0] = ((uint8_t)(8 * rgb565) >> 5) | (8 * rgb565);
    color.rgba[2] = ((uint8_t)(8 * (rgb565 >> 11)) >> 5) | (8 * (rgb565 >> 11));
    *pcolor = color;
}
