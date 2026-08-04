#pragma once

/* Havok status code. Only HK_SUCCESS is referenced by name in the reconstructed
   corpus; it is the zero/false branch value (a successful flatten returns 0 and
   takes the compute path). HK_FAILURE is the standard non-zero counterpart. */
typedef enum hkResult
{
    HK_SUCCESS = 0,
    HK_FAILURE = 1
} hkResult;
