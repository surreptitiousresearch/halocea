/* hs_scenario_merge @0x8372EB20 — merge a slave scenario's HS source files into the master: for each
 * slave source file (52-byte records: null-terminated name at +0, a tag_data sub-block at +32) not already
 * present by name in the master's list, appends a new element, copies the name, resizes the new element's
 * tag_data to match, and copies the source text. Always drops (resizes to 0) the master's hs_scripts block
 * — compiled scripts are invalidated by any source merge and must be recompiled. Returns 0 if any merge
 * step failed (element/tag_data allocation), else 1. */

#include <stdint.h>
#include <string.h>
#include "headers/scenario.h"
#include "headers/hs_source_file.h"
#include "headers/tag_data.h"
#include "headers/tag_block.h"
extern int16_t tag_block_add_element(tag_block *block);

/* strcpy declared by <string.h> */
/* memcpy declared by <string.h> */

int hs_scenario_merge(scenario *master, const scenario *slave)
{
    int success = 1;

    for ( int16_t i = 0; i < slave->hs_source_files.count; i++ )
    {
        const hs_source_file *slave_source =
            &((const hs_source_file *)slave->hs_source_files.address)[i];

        int16_t j = 0;
        while ( j < master->hs_source_files.count
             && stricmp(slave_source->name,
                        ((const hs_source_file *)master->hs_source_files.address)[j].name) )
        {
            j++;
        }

        if ( j == master->hs_source_files.count )
        {
            int16_t new_index = tag_block_add_element(&master->hs_source_files);
            hs_source_file *new_source =
                &((hs_source_file *)master->hs_source_files.address)[new_index];
            const tag_data *slave_data = &slave_source->source;

            if ( new_index == -1 )
            {
                success = 0;
            }
            else
            {
                strcpy(new_source->name, slave_source->name);
                tag_data *new_data = &new_source->source;
                if ( !tag_data_resize(new_data, slave_data->size) )
                {
                    success = 0;
                }
                else
                {
                    memcpy(new_data->address, slave_data->address, slave_data->size);
                }
            }
        }
    }

    tag_block_resize(&master->hs_scripts, 0);
    return success;
}
