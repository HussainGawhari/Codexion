int error_exit(const char *msg, const char *details, int exit_code)
{
    if (details)
        printf(msg, details);
    else
        printf("[ERROR]] %s.\n", msg);
    return (exit_code);
}
