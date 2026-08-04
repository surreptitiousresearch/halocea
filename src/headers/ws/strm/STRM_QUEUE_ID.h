#pragma once
// ws-engine strm (streaming): which physical device queue a strmRESOURCE streams through.
// DB-verified (types_enum_values STRM_QUEUE_ID).

enum STRM_QUEUE_ID {
    STRM_QUEUE_DVD   = 0,
    STRM_QUEUE_HDD   = 1,
    STRM_QUEUE_COUNT = 2,
};
