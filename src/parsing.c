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
        data->scheduler = get_scheduler();
    else
        return (print_error(STR_ERR_INPUT_SCHEDULER, av[8]));
    return (true);
}

/**
 * here i am trying to inilized basic data
 */

bool parse_data(t_data *data, int *args)
{
    data->coders = malloc(sizeof(t_coder) * args[0]);
    if (!data->coders)
        return (print_error(STR_ERR_MALLOC, NULL));
    data->dongles = malloc(sizeof(data->dongles) * args[0]);
    if (!data->dongles)
        return (print_error(STR_ERR_MALLOC, NULL));
    data->nb_coders = args[0];
    data->burnout_time = args[1];
    data->compile_time = args[2];
    data->debug_time = args[3];
    data->refactor_time = args[4];
    data->nb_compiles = args[5];
    data->dongle_cooldown = args[6];
    return (true);
    
}
