#pragma once
/* d3dCOND_SURVEY_RES — one conditional-survey (occlusion query) result entry (DB types_members
 * d3dCOND_SURVEY_RES, 8 bytes). */

typedef struct objOBJ objOBJ;

typedef struct d3dCOND_SURVEY_RES
{
    objOBJ       *pObj;   /* 0x00 */
    unsigned int *buffer; /* 0x04 */
} d3dCOND_SURVEY_RES; /* 8 bytes */
