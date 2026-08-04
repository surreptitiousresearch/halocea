extern void hs_tokens_enumerate_add_string(const char *string);

void hs_enumerate_special_form_names(void)
{
    hs_tokens_enumerate_add_string("script");
    hs_tokens_enumerate_add_string("global");
}
