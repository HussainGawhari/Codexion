#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>
# include <string.h>

# define MAX_CODERS			250
# define STR_MAX_CODERS		"250"

# ifndef DEBUG_FORMATTING
#  define DEBUG_FORMATTING	0
# endif

# define RST      "\033[0m"
# define RED      "\033[31m"
# define GREEN    "\033[32m"
# define PURPLE   "\033[35m"
# define CYAN     "\033[36m"

# define STR_PROG_NAME	"codexion:"

# define STR_USAGE	"./codexion <number_of_coders> <time_to_burnout> \
					<time_to_compile> <time_to_debug> <time_to_refactor> \
					<number_of_compiles_required> <dongle_cooldown> <scheduler>"

// invalid integers
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"

// Invlide input
# define STR_ERR_INPUT_COFLOW	"%s invalid input: \
there must be between 1 and %s coders.\n"

// Thread creation error
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"
# define STR_ERR_INPUT_COUNT "%s you need to have 9 arguments"
# define STR_ERR_INPUT_OVERFLOW "%s you have to many inputs inputs"
# define STR_ERR_INPUT_VALUE "%s you have invalid inputs"
# define STR_ERR_INPUT_SCHEDULER "%s provide valid scheduler for me"
// Action
# define STR_BURNOUT	"died"
# define STR_DONGLE		"has taken a dongle"
# define STR_COMPILE	"is compiling"
# define STR_DEBUG		"is debugging"
# define STR_REFACTOR	"is refactoring"

// Max coders 
# define MAX_CODERS 300

// Scheduler
# define FIFO "fifo"
# define EDF  "edf"

typedef struct s_data			t_data;
typedef struct s_coder			t_coder;
typedef struct s_dongle			t_dongle;
typedef struct s_queue			t_queue;
typedef struct s_queue_manager	t_queue_manager;
typedef struct s_heap			t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
}	t_dongle;


typedef struct s_args
{
	int		nb_coders;
	int		burnout_time; // in ms
	int		compile_time; // in ms
	int		debug_time; // in ms
	int		refactor_time; // in ms
	int		nb_compiles;
	int		dongle_cooldown; // in ms
	bool	scheduler; // 0 = edf, 1 = fifo
}	t_args;

typedef struct s_coder
{
	int				id;
	long			last_compile;
	int				compile_count;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	struct s_data	*data;
}	t_coder;

typedef struct s_data
{
	t_dongle		*dongles;
	t_coder			*coders;
	int		nb_coders;
	int		burnout_time; // in ms
	int		compile_time; // in ms
	int		debug_time; // in ms
	int		refactor_time; // in ms
	int		nb_compiles;
	int		dongle_cooldown; // in ms
	char	*scheduler;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	
}	t_data;

/* Parsing */
int		is_valid_input(t_data *data, char **av);
int		ft_atoi(const char *str);

/* Init */
t_data		*init_data(int ac, char **av, int i);
int		init_coders(t_data *data);

/* Threads */
int		start_simulation(t_data *data);
void	*routine(void *arg);
void	*monitor(void *arg);

/* Utils */
long	get_time_ms(void);
void	ft_usleep(long time);
void	print_status(t_coder *coder, char *msg);

/* Cleanup */
void	destroy_all(t_data *data);
int	print_error(const char *msg, char *details);

#endif
