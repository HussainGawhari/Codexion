#include "codexion.h"

/*
*   compare both the string for scheduler
*   if both string are not equal it will return non zero

*/
static int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/* is_valid_input:
*	Checks if all required arguments are valid, i.e. is a string of
*	digits only, which does not exceed INT MAX. Also checks if the number
*	of philosophers is valid (between 1 and MAX_PHILOS).
*	Returns true if all arguments are valid, false if one of them is invalid.
*/


bool is_valid_input(t_data *data, char **av)
{
    int parsed_args[8];
    int i;

    i = 1;
    while (i < 8)
    {
        parsed_args[i - 1] = ft_atoi(av[i]);
        if (parsed_args[i - 1] == -1)
            return (print_error(STR_ERR_INPUT_OVERFLOW, av[i]));
        if (parsed_args[i - 1] <= 0)
            return (print_error(STR_ERR_INPUT_VALUE, av[i]));
        if (i == 1 && parsed_args[0] > MAX_CODERS)
            return (print_error(STR_ERR_INPUT_COFLOW, STR_MAX_CODERS));
        i++;
    }
    if (!parse_data(data, parsed_args))
        return (false);
    if (ft_strcmp(av[8], FIFO) == 0 || ft_strcmp(av[8], EDF) == 0)
        parsed_args[i -1] = av[i];
    else
        return (print_error(STR_ERR_INPUT_SCHEDULER, av[8]));
    return (true);
}

/**
 * here i am trying to inilized basic data
 * return true if everything goes well
 * stop if memory allocation fails
 */

bool parse_data(t_data *data, int *args)
{
    data->coders = malloc(sizeof(t_coder) * args[0]);
    if (!data->coders)
        return (print_error(STR_ERR_MALLOC, NULL));
    data->dongles = malloc(sizeof(data->dongles) * args[0]);
    if (!data->dongles)
        return (print_error(STR_ERR_MALLOC, NULL));
    data->number_of_coders = args[0];
    data->time_to_burnout = args[1];
    data->time_to_compile = args[2];
    data->time_to_debug = args[3];
    data->time_to_refactor = args[4];
    data->number_of_compiles_required = args[5];
    data->dongle_cooldown = args[6];
    data->scheduler = args[7];
    return (true);
}
