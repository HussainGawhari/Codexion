/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:28:41 by hgawhari          #+#    #+#             */
/*   Updated: 2026/07/24 10:33:45 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	cmp_edf(t_request a, t_request b)
{
	if (a.deadline < b.deadline)
		return (-1);
	if (a.deadline > b.deadline)
		return (1);
	// return (cmp_fifo) we call it here too to make sure to choose a scheduler
	return (0);
}
