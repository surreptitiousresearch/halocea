/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* hcex_dump_all_models @0x83682970 — HCEX debug dumper. Walks the loaded tag database by group tag and
 * prints names (and, for models, their region/permutation/node hierarchy) to the debug console via
 * hcex_output_dbg. Each of the five sub-dumps is gated by its own debug flag so the developer can enable
 * exactly the listing they want. Pure diagnostic output — no game state is modified.
 *
 * Tag definition pointers are fetched as *(int *)TAG_INSTANCE(index); model region/permutation/node blocks
 * are walked at raw offsets into the (opaque) gbxmodel tag definition. */

#include <stdint.h>
#include "../headers/global_tag_instances.h"
#include "../headers/tag_iterator.h"
#include "../headers/model.h"
#include "../headers/model_node.h"
#include "../headers/model_region.h"
#include "../headers/model_region_permutation.h"

extern uint8_t dump_decals;
extern uint8_t dump_perm;
extern uint8_t dump_effe;
extern uint8_t dump_model_names;
extern uint8_t dump_model_nodes;

extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);
extern char *tag_get_name(int16_t tag_index);
extern void hcex_output_dbg(const char *text, ...);
extern void hcex_dump_mdl_name(const char *mdl_name);

extern "C" void hcex_dump_all_models(void)
{
    tag_iterator iterator;
    int tag_index;

    if ( dump_decals )
    {
        tag_iterator_new(&iterator, 0x64656361 /* 'deca' */);
        hcex_output_dbg("decals: \n");
        for ( tag_index = tag_iterator_next(&iterator); tag_index != -1;
              tag_index = tag_iterator_next(&iterator) )
        {
            int decal_definition = *(int *)TAG_INSTANCE(tag_index);
            const char *name = tag_get_name(tag_index);
            const char *bitmap_name = *(const char **)(decal_definition + 220);
            if ( !bitmap_name )
                bitmap_name = "null";
            hcex_output_dbg("%s\n %s\n", name, bitmap_name);
        }
    }

    if ( dump_perm )
    {
        tag_iterator_new(&iterator, 0x6D6F6432 /* 'mod2' */);
        hcex_output_dbg("models: \n");
        for ( tag_index = tag_iterator_next(&iterator); tag_index != -1;
              tag_index = tag_iterator_next(&iterator) )
        {
            model *model_definition = *(model **)TAG_INSTANCE(tag_index);
            const char *name = tag_get_name(tag_index);
            if ( !name )
                continue;
            /* Only list models that have permutation variety: >1 region, or 1 region with >1 permutation. */
            int region_count = model_definition->regions.count;
            if ( region_count < 1
              || (region_count == 1
                  && ((model_region *)model_definition->regions.address)[0].permutations.count <= 1) )
                continue;

            hcex_output_dbg("%s\n", name);
            for ( int region_index = 0; region_index < model_definition->regions.count;
                  region_index = (int16_t)(region_index + 1) )
            {
                model_region *region = &((model_region *)model_definition->regions.address)[region_index];
                hcex_output_dbg("  %s\n", region->name);
                model_region_permutation *permutation = (model_region_permutation *)region->permutations.address;
                for ( int permutation_index = 0; permutation_index < region->permutations.count;
                      ++permutation_index )
                {
                    hcex_output_dbg("      %s\n", permutation->name);
                    ++permutation;
                }
            }
        }
    }

    if ( dump_effe )
    {
        tag_iterator_new(&iterator, 0x65666665 /* 'effe' */);
        hcex_output_dbg("effects: \n");
        for ( tag_index = tag_iterator_next(&iterator); tag_index != -1;
              tag_index = tag_iterator_next(&iterator) )
        {
            const char *name = tag_get_name(tag_index);
            if ( name )
                hcex_output_dbg("%s\n", name);
        }

        tag_iterator_new(&iterator, 0x7063746C /* 'pctl' */);
        hcex_output_dbg("part sys: \n");
        for ( tag_index = tag_iterator_next(&iterator); tag_index != -1;
              tag_index = tag_iterator_next(&iterator) )
        {
            const char *name = tag_get_name(tag_index);
            if ( name )
                hcex_output_dbg("%s\n", name);
        }

        tag_iterator_new(&iterator, 0x70617274 /* 'part' */);
        hcex_output_dbg("particles: \n");
        for ( tag_index = tag_iterator_next(&iterator); tag_index != -1;
              tag_index = tag_iterator_next(&iterator) )
        {
            const char *name = tag_get_name(tag_index);
            if ( name )
                hcex_output_dbg("%s\n", name);
        }
    }

    if ( dump_model_names )
    {
        tag_iterator_new(&iterator, 0x6D6F6432 /* 'mod2' */);
        hcex_output_dbg("models: \n");
        for ( tag_index = tag_iterator_next(&iterator); tag_index != -1;
              tag_index = tag_iterator_next(&iterator) )
        {
            const char *name = tag_get_name(tag_index);
            if ( name )
                hcex_dump_mdl_name(name);
        }
    }

    if ( dump_model_nodes )
    {
        tag_iterator_new(&iterator, 0x6D6F6432 /* 'mod2' */);
        hcex_output_dbg("models: \n");
        for ( tag_index = tag_iterator_next(&iterator); tag_index != -1;
              tag_index = tag_iterator_next(&iterator) )
        {
            model *model_definition = *(model **)TAG_INSTANCE(tag_index);
            const char *name = tag_get_name(tag_index);
            if ( !name )
                continue;
            hcex_dump_mdl_name(name);
            hcex_output_dbg(name);
            hcex_output_dbg("\n");
            if ( !model_definition )
                continue;
            for ( int node_index = 0; node_index < model_definition->nodes.count;
                  node_index = (int16_t)(node_index + 1) )
            {
                const char *node = ((model_node *)model_definition->nodes.address)[node_index].name;
                hcex_output_dbg("  ");
                hcex_output_dbg(node);                                     /* node name @+0 */
                hcex_output_dbg("\n");
            }
        }
    }
}
