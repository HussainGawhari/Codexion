/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:03:59 by 42header.us       #+#    #+#             */
/*   Updated: 2026/07/28 16:27:04 by hgawhari         ###   ########.fr       */
/*                                    S                                        */
/* ************************************************************************** */

#include "codexion.h"

int main(int ac, char **av)
{
    t_data data;
    memset(&data, 0, sizeof(t_data));
    if (ac != 9)
        return (print_error(STR_ERR_INPUT_COUNT, NULL), 0);
    if (!is_valid_input(&data, av))
        return (EXIT_FAILURE);
    init_data(&data);
    start_simulation(&data);
    cleanup(&data);
    return (0);
}
