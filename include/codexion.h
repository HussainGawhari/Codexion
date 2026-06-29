#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

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

# define STR_USAGE	"%s usage: ./codexion <number_of_coders> \
<time_to_burnout> <time_to_compile> <time_to_debug> \
[number_of_times_each_coder_must_compile]\n"

# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"

# define STR_ERR_INPUT_COFLOW	"%s invalid input: \
there must be between 1 and %s coders.\n"

# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"

# define STR_BURNOUT	"died"
# define STR_DONGLE		"has taken a dongle"
# define STR_COMPILE	"is compiling"
# define STR_DEBUG		"is debugging"
# define STR_REFACTOR	"is refactoring"


typedef struct s_dongle
{
	pthread_mutex_t	mutex;
}	t_dongle;

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
	int				nb_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	int				must_compile;
	int				stop;
	long			start_time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_data;

/* Parsing */
int		parse_args(int ac, char **av, t_data *data);

/* Init */
int		init_data(t_data *data);
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

#endif
