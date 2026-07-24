/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:06:27 by hgawhari          #+#    #+#             */
/*   Updated: 2026/07/24 16:53:33 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define MAX_CODERS 250
#define STR_MAX_CODERS "250"

#ifndef DEBUG_FORMATTING
#define DEBUG_FORMATTING 0
#endif

#define RST "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define PURPLE "\033[35m"
#define CYAN "\033[36m"

#define STR_PROG_NAME "codexion:"

#define STR_USAGE "./codexion <number_of_coders> <time_to_burnout> \
					<time_to_compile> <time_to_debug> <time_to_refactor> \
					<number_of_compiles_required> <dongle_cooldown> <scheduler>"

// invalid integers
#define STR_ERR_INPUT_DIGIT "%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"

// Invlide input
#define STR_ERR_INPUT_COFLOW "%s invalid input: \
there must be between 1 and %s coders.\n"

// Thread creation error
#define STR_ERR_THREAD "%s error: Could not create thread.\n"
#define STR_ERR_MALLOC "%s error: Could not allocate memory.\n"
#define STR_ERR_MUTEX "%s error: Could not create mutex.\n"
#define STR_ERR_INPUT_COUNT "%s you need to have 9 arguments"
#define STR_ERR_INPUT_OVERFLOW "%s you have to many inputs inputs"
#define STR_ERR_INPUT_VALUE "%s you have invalid inputs"
#define STR_ERR_INPUT_SCHEDULER "%s provide valid scheduler for me"
// Action
#define STR_BURNOUT "died"
#define STR_DONGLE "has taken a dongle"
#define STR_COMPILE "is compiling"
#define STR_DEBUG "is debugging"
#define STR_REFACTOR "is refactoring"

// Max coders
#define MAX_CODERS 300

// Scheduler
//#define FIFO "fifo"
//#define EDF "edf"

typedef enum e_scheduler
{
	FIFO,
	EDF,
} t_scheduler;

typedef struct s_request
{
	unsigned int coder_id;
	long deadline;
	unsigned long arrival_order;
} t_request;

typedef struct s_node
{
	t_request req;
	struct s_node *next;
} t_node;

typedef struct s_queue
{
	t_node *head;
	int size;
	int (*cmp)(t_request, t_request);
} t_queue;

typedef struct s_dongle
{
	pthread_mutex_t mutex;
	bool available;
	long last_release_ms;
	t_queue wait_queue;
	pthread_cond_t cond;
} t_dongle;

typedef struct s_coder
{
	unsigned int id;
	unsigned int compiles_done;
	long last_compile_ms;
	t_dongle *left;
	t_dongle *right;
	int left_idx;
	int right_idx;
	pthread_t thread;
	pthread_mutex_t mutex;
	struct s_data *data;
} t_coder;

typedef struct s_data
{
	unsigned int number_of_coders;
	unsigned int time_to_burnout;
	unsigned int time_to_compile;
	unsigned int time_to_debug;
	unsigned int time_to_refactor;
	unsigned int number_of_compiles_required;
	unsigned int dongle_cooldown;
	t_scheduler scheduler;
	t_coder *coders;
	t_dongle *dongles;
	pthread_mutex_t log_mutex;
	pthread_mutex_t simulation_mutex;
	bool running;
	unsigned long start_time;
	unsigned long request_counter;
	pthread_mutex_t counter_mutex;
	pthread_t monitor_thread;
} t_data;

/* Parsing */
int is_valid_input(t_data *data, char **av);
int ft_atoi(const char *str);

/*scheduler*/
int cmp_fifo(t_request a, t_request b);
int	cmp_edf(t_request a, t_request b);

/* Init */
void init_data(t_data *data);
int init_coders(t_data *data);

/*monitoring*/
void monitor_routine(void *arg);
void	*coder_routine(void *arg);

/* Threads */
void start_simulation(t_data *data);
void *routine(void *arg);
void *monitor(void *arg);

/* Utils */
long get_time_ms(void);
void ft_usleep(long time, t_data *data);
void print_status(t_coder *coder, char *msg);

/* Cleanup */
void destroy_all(t_data *data);
int print_error(const char *msg, char *details);

#endif
