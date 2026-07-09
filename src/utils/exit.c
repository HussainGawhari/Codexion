#include "codexion.h"

int print_error(const char *msg, char *details)
{
    if (*details)
        printf(msg, details);
    else
        print(msg);
    return (0);
}

/* error_null:
*	Frees any allocated memory, prints an error message and returns a NULL pointer.
*	Used for error management during initialization.
*/
void	*error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}