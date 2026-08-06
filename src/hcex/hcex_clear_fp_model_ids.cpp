/* hcex_clear_fp_model_ids @0x823CE790 — reset the two cached first-person model-id slots (one
 * per split-screen camera) for each of the two tracked model registers. Paired with
 * hcex_get_fp_matrices, which reads these same arrays to find a matching cached matrix set. */

extern "C" int hcex_fp_model1[2];
extern "C" int hcex_fp_model2[2];

void hcex_clear_fp_model_ids(void)
{
    hcex_fp_model1[0] = 0;
    hcex_fp_model1[1] = 0;
    hcex_fp_model2[0] = 0;
    hcex_fp_model2[1] = 0;
}
