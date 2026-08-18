/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 16:42:20 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/10 19:19:40 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
	* This is the function which make coders acquire dongles and compile release and repeat
*/

static int compile_cycle(t_coder *coder, t_data *data)
{
	if (!acquire_dongles(coder, data))
		return 0;

	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_ms = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);

	log_event(data, coder->id, "is compiling");

	ft_usleep(data->time_to_compile, data);
	release_dongles_for_coder(coder);

	pthread_mutex_lock(&coder->mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->mutex);
	return 1;
}

/*
	* This function controles the coder work cycles
*/

void	*coder_routine(void *arg)
{
	t_coder *coder;
	t_data *data;

	coder = (t_coder *) arg;
	data = coder->data;
	while (simulation_is_running(data) && coder->compiles_done < data->number_of_compiles_required)
	{
		if (!compile_cycle(coder, data))
			break;
		log_event(data, coder->id, "is debugging");
		ft_usleep(data->time_to_debug, data);
		log_event(data, coder->id, "is refactoring");
		ft_usleep(data->time_to_refactor, data);
	}
	return (NULL);
}
