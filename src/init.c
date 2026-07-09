#include "codexion.h"

t_data		*init_data(int ac, char **av)
{
    t_data *data;

    data = malloc(sizeof(t_data) * 1);
    if (!data)
        return (error_null(STR_ERR_MALLOC, NULL, 0));
    data->dongles = integer_atoi(av[0]);
    data->nb_coders = integer_atoi(av[0]);
    data->burnout_time = av[1];
    data->compile_time = av[2];
    data->debug_time = av[3];
    data->refactor_time = av[4];
    data->nb_compiles = av[5];
    data->dongle_cooldown = av[6];
    data->scheduler = av[7];
    return (data);
}