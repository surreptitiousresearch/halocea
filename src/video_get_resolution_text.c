/* video_get_resolution_text @0x83785208 */
#include <stdint.h>
#include "headers/video_resolution_s.h"

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */

uint16_t * video_get_resolution_text(int mode_index)
{
    if ( mode_index < 0 || mode_index >= video_resolution_count )
        return (uint16_t *)empty_wide_string;   /* .rdata const -> the corpus's uint16_t* return shape */
    return video_resolutions[mode_index].name;
}
