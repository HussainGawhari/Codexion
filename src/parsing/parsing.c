/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 07:48:38 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/16 14:03:37 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*
    * check if the integer we receives are positive or not
*/
static bool is_positive_integer(const char *str)
{
    if (!str || !*str)
        return (false);
    while (*str)
    {
        if (*str < '0' || *str > '9')
            return (false);
        str++;
    }
    return (true);
}


/**
 * here i am trying to inilized basic data
 * return true if everything goes well
 * stop if memory allocation fails
 */

static bool parse_data(t_data *data, int *args)
{
    data->coders = malloc(sizeof(t_coder) * args[0]);
    if (!data->coders)
    {
        print_error(STR_ERR_MALLOC, NULL);
        return (false);
    }
    data->dongles = malloc(sizeof(t_dongle) * args[0]);
    if (!data->dongles)
    {
        print_error(STR_ERR_MALLOC, NULL);
        free(data->coders);
        return (false);
    }
    data->number_of_coders = args[0];
    data->time_to_burnout = args[1];
    data->time_to_compile = args[2];
    data->time_to_debug = args[3];
    data->time_to_refactor = args[4];
    data->number_of_compiles_required = args[5];
    data->dongle_cooldown = args[6];
    /* scheduler set from argv[8] in is_valid_input */
    return (true);
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
        if (!is_positive_integer(av[i]))
        {
            print_error(STR_ERR_INPUT_VALUE, av[i]);
            return (false);
        }
        parsed_args[i - 1] = ft_atoi(av[i]);
        if (parsed_args[i - 1] == -1)
        {
            print_error(STR_ERR_INPUT_OVERFLOW, av[i]);
            return (false);
        }
        if (parsed_args[i - 1] < 0)
        {
            print_error(STR_ERR_INPUT_VALUE, av[i]);
            return (false);
        }
        if (i == 1 && parsed_args[0] > MAX_CODERS)
        {
            print_error(STR_ERR_INPUT_COFLOW, STR_MAX_CODERS);
            return (false);
        }
        i++;
    }
    if (!parse_data(data, parsed_args))
        return (false);
    if (ft_strcmp(av[8], STR_SCHED_FIFO) == 0)
        data->scheduler = FIFO;
    else if (ft_strcmp(av[8], STR_SCHED_EDF) == 0)
        data->scheduler = EDF;
    else
    {
        print_error(STR_ERR_INPUT_SCHEDULER, av[8]);
        return (false);
    }
    return (true);
}
