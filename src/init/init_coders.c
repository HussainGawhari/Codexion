/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 11:38:43 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/18 13:34:48 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
	* Initializes every coders and connect to neighboring dongles
	* loops through every coder and give id to it
	* The indexes are determine which dongle should be aquired first.
	* Create mutex for each coder to safly access share coder data.

*/
void	init_coders(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i] = (t_coder){
			.id = i + 1,
			.compiles_done = 0,
			.last_compile_ms = 0,
			.left = &data->dongles[i],
			.right = &data->dongles[(i + 1) % data->number_of_coders],
			.left_idx = (int)i,
			.right_idx = (int)((i + 1) % data->number_of_coders),
			.data = data,
		};
		pthread_mutex_init(&data->coders[i].mutex, NULL);
		i++;
	}
}
