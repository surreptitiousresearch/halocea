/* compare_video_resolutions @0x83785038 */
#include "headers/video_resolution_s.h"

int compare_video_resolutions(const video_resolution_s *res_1, const video_resolution_s *res_2)
{
    if (res_1->width < res_2->width)
        return -1;
    if (res_1->width > res_2->width)
        return 1;
    if (res_1->height < res_2->height)
        return -1;
    if (res_1->height > res_2->height)
        return 1;
    return 0;
}
