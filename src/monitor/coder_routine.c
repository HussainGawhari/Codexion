/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 16:42:20 by hgawhari          #+#    #+#             */
/*   Updated: 2026/07/24 16:48:57 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder *coder;
	t_data *data;

	coder = (t_coder *) arg;
	data = coder->data;
	while (is_running(data) && coder->compiles_done < data->number_of_compiles_required)
	{
		if (!compile_cycle(coder, data))
			break;
		log_action(data, coder->id, "is debugging");
		ft_usleep(data->time_to_debug, data);
		log_action(data, coder->id, "is refactoring");
		ft_usleep(data->time_to_refactor, data);
	}
	return (NULL);
}
