/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 13:31:35 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/18 13:31:43 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 *Entery point to the codexion
 */
int	main(int ac, char **av)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (ac != 9)
		return (print_error(STR_ERR_INPUT_COUNT, NULL), 0);
	if (!is_valid_input(&data, av))
		return (print_error(STR_ERR_INPUT_VALUE, NULL), 0);
	init_data(&data);
	start_simulation(&data);
	cleanup(&data);
	return (0);
}
