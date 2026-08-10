/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:28:41 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/10 19:23:40 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	cmp_edf(t_task a, t_task b)
{
	if (a.deadline < b.deadline)
		return (-1);
	if (a.deadline > b.deadline)
		return (1);
	return (cmp_fifo(a, b));
}
