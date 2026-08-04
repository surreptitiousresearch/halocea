#pragma once
/* hs_tag_reference — element of scenario.hs_references (40 bytes). Layout from the database `hs_tag_reference`
 * type: a runtime resolution cache followed by the tag_reference itself. */

#include "tag_reference.h"

typedef struct hs_tag_reference
{
    int           runtime_instance_index;  /* 0x00 */
    int           runtime_timestamp;       /* 0x04 */
    int           unused[4];               /* 0x08 */
    tag_reference reference;               /* 0x18 */
} hs_tag_reference;                         /* 40 bytes */

/* Group tag expected for each tag-reference script type, indexed by (script type - 24). */
extern const unsigned int hs_tag_reference_type_group_tags[];
