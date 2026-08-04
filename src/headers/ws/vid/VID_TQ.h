#pragma once
/* VID_TQ — per-render-pass GPU time-query id. One entry per timed pass in the frame; used to
 * index the video driver's hardware GPU-timer slots and the matching gsRendCnt_GPU_* counters.
 * DB-verified (types_enum_values VID_TQ). */

typedef enum VID_TQ
{
    VID_TQ_Z_PREPASS                                  = 0,
    VID_TQ_DISTORTION_MASK                            = 1,
    VID_TQ_SHADOWMAP0                                 = 2,
    VID_TQ_SHADOWMAP1                                 = 3,
    VID_TQ_SHADOWMAP2                                 = 4,
    VID_TQ_SHADOWMAP3                                 = 5,
    VID_TQ_SHADOWMAP                                  = 6,
    VID_TQ_SSAO                                       = 7,
    VID_TQ_FOG_MASK                                   = 8,
    VID_TQ_LBUF                                       = 9,
    VID_TQ_FRAME_OPAQUE                               = 10,
    VID_TQ_FRAME_TRANSP_SCORCH_SKY__PART_AKILL        = 11,
    VID_TQ_FRAME_MSG_REND                             = 12,
    VID_TQ_FRAME_PART                                 = 13,
    VID_TQ_POST_PROC_FLARES_EDGE_AA__DISTORT_MBLUR    = 14,
    VID_TQ_POST_PROC_SCREEN_EFFECTS                   = 15,
    VID_TQ_GUI                                        = 16,
    VID_TQ_UPSAMPLE                                   = 17,
    VID_TQ_LAST                                       = 18,
} VID_TQ;

// Advance a VID_TQ in a for-loop (the harvest loop iterates VID_TQ_Z_PREPASS..VID_TQ_LAST).
inline VID_TQ &operator++(VID_TQ &q) { q = (VID_TQ)(q + 1); return q; }
