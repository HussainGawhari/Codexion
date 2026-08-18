/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:25:27 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/18 11:11:01 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * Release one dongle and wake all threads waiting for it.
 * Updates its availability and records the release time.
 */
static void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = true;
	dongle->last_release_ms = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

/*
 * Release both dongles currently assigned to a coder.
 * Each dongle is released independently and its waiters are notified.
 */
void	release_dongles_for_coder(t_coder *coder)
{
	release_dongle(coder->left);
	release_dongle(coder->right);
}
