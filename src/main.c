#include "codexion.h"

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    t_data *data;
    if (ac -1 < 4 || ac -1 > 5)
        return (error_exit(STR_USAGE, NULL, EXIT_FAILURE));
    if (!is_valid_input(ac, av))
        return (EXIT_FAILURE);
    data = init_data(ac, av, 1);
    if (!data)
        return (EXIT_FAILURE);
    if (!start_simulation(data))
        return (EXIT_FAILURE);
    return 0;
}
