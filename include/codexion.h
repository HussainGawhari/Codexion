/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:06:27 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/17 19:38:24 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

/* =============================== Includes =============================== */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

/* ================================ Macros ================================ */

#define MAX_CODERS 250
#define STR_MAX_CODERS "250"

#ifndef DEBUG_FORMATTING
# define DEBUG_FORMATTING 0
#endif

/* Colors */
#define RST     "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define PURPLE  "\033[35m"
#define CYAN    "\033[36m"

/* Program strings */
#define STR_PROG_NAME "codexion:"

#define STR_USAGE "./codexion <number_of_coders> <time_to_burnout> \
					<time_to_compile> <time_to_debug> <time_to_refactor> \
					<number_of_compiles_required> <dongle_cooldown> <scheduler>"

/* Error messages */
#define STR_ERR_INPUT_DIGIT "%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"

#define STR_ERR_INPUT_COFLOW "%s invalid input: \
there must be between 1 and %s coders.\n"

#define STR_ERR_THREAD "%s error: Could not create thread.\n"
#define STR_ERR_MALLOC "%s error: Could not allocate memory.\n"
#define STR_ERR_MUTEX "%s error: Could not create mutex.\n"
#define STR_ERR_INPUT_COUNT "%s you need to have 9 arguments"
#define STR_ERR_INPUT_OVERFLOW "%s you have to many inputs inputs"
#define STR_ERR_INPUT_VALUE "%s you have invalid inputs"
#define STR_ERR_INPUT_SCHEDULER "%s provide valid scheduler for me"

/* Actions */
#define STR_BURNOUT   "died"
#define STR_DONGLE    "has taken a dongle"
#define STR_COMPILE   "is compiling"
#define STR_DEBUG     "is debugging"
#define STR_REFACTOR  "is refactoring"

/* Scheduler strings */
#define STR_SCHED_FIFO "fifo"
#define STR_SCHED_EDF  "edf"

/* =============================== Scheduler ============================== */

typedef enum e_scheduler
{
	FIFO,
	EDF,
} t_scheduler;

/* =============================== Structures ============================= */

typedef struct s_task
{
	unsigned int      coder_id;
	long              deadline;
	unsigned long     arrival_order;
} t_task;

typedef struct s_node
{
	t_task         req;
	struct s_node    *next;
} t_node;

typedef struct s_queue
{
	t_node            *head;
	int                size;
	int (*cmp)(t_task, t_task);
} t_queue;

typedef struct s_dongle
{
	pthread_mutex_t   mutex;
	bool              available;
	long              last_release_ms;
	t_queue           wait_queue;
	pthread_cond_t    cond;
} t_dongle;

typedef struct s_coder
{
	unsigned int      id;
	unsigned int      compiles_done;
	long              last_compile_ms;
	t_dongle         *left;
	t_dongle         *right;
	int                left_idx;
	int                right_idx;
	pthread_t         thread;
	pthread_mutex_t   mutex;
	struct s_data    *data;
} t_coder;

typedef struct s_data
{
	unsigned int      number_of_coders;
	unsigned int      time_to_burnout;
	unsigned int      time_to_compile;
	unsigned int      time_to_debug;
	unsigned int      time_to_refactor;
	unsigned int      number_of_compiles_required;
	unsigned int      dongle_cooldown;
	t_scheduler       scheduler;
	t_coder           *coders;
	t_dongle          *dongles;

	pthread_mutex_t   log_mutex;
	pthread_mutex_t   simulation_mutex;
	bool              running;

	unsigned long    start_time;
	unsigned long    request_counter;

	pthread_mutex_t   counter_mutex;
	pthread_t         monitor_thread;
} t_data;

/* ================================ Parsing =============================== */

bool is_valid_input(t_data *data, char **av);
int  ft_atoi(const char *str);

/* =============================== Comparators ============================ */

int cmp_fifo(t_task a, t_task b);
int cmp_edf(t_task a, t_task b);

/* ================================ Init ================================= */

void init_data(t_data *data);
void init_dongles(t_data *data);
void init_coders(t_data *data);

/* ============================ Monitoring/Threads ========================= */

void *monitor_simulation(void *arg);
void *coder_routine(void *arg);

/* Internal to main.c (declared here only if you need it elsewhere) */
void start_simulation(t_data *data);
void wake_all_dongles(t_data *data);

/* ================================ Queue ================================= */

void queue_init(t_queue *q, int (*cmp)(t_task, t_task));
t_task queue_top(t_queue *q);
void queue_push(t_queue *q, t_task req);
void queue_pop(t_queue *q);
void queue_destroy(t_queue *q);

/* =========================== Dongle/Queue helpers ======================== */

bool acquire_dongles(t_coder *coder, t_data *data);
void release_dongles_for_coder(t_coder *coder);
void get_left_right_dongles(t_coder *coder, t_dongle **first, t_dongle **second);
int  is_dongle_ready(t_dongle *dongle, t_data *data);

/* ================================ Utils ================================= */

int  simulation_is_running(t_data *data);
void log_event(t_data *data, unsigned int id, char *action);

unsigned long get_time_ms(void);
void ft_usleep(unsigned long time, t_data *data);

/* =============================== Cleanup ================================= */

void cleanup(t_data *data);
int  print_error(const char *msg, char *details);

#endif
