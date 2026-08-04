#include "headers/video_resolution_s.h"

int video_resolution_find(unsigned int width, unsigned int height)
{
    for (int i = 0; i < video_resolution_count; i++)
    {
        if (video_resolutions[i].width == width && video_resolutions[i].height == height)
            return i;
    }
    return -1;
}
