#include "codexion.h"

int main(int ac, char **av)
{
    t_data *data;
    // data = NULL;

    if (ac != 9)
        return(print_error(STR_ERR_INPUT_COUNT, NULL),0);
    if (!is_valid_input(&data, av))
        return (EXIT_FAILURE);
    data = init_data(ac, av, 1);
    if (!data)
        return (EXIT_FAILURE);
    if (!start_simulation(data))
        return (EXIT_FAILURE);
    return 0;
}
