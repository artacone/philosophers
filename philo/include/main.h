/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 17:45:14 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/22 17:45:31 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>

# define MSG_FORK	"has taken a fork"
# define MSG_EAT	"is eating"
# define MSG_SLEEP	"is sleeping"
# define MSG_THINK	"is thinking"
# define MSG_DEATH	"died"

// Consider unsigned
typedef struct s_input {
	int		n_philos;
	int		t_to_die;
	int		t_to_eat;
	int		t_to_sleep;
	int		n_to_eat;
	int		hungry;
	size_t	t_start;
}	t_input;

typedef struct s_philo {
	int				id;
	int				is_alive;
	int				meals_left;
	size_t			t_last_meal;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
	pthread_mutex_t	*print;
	pthread_mutex_t	*time;
	t_input			*input;
	int				*ok;
}	t_philo;

typedef struct s_table {
	int				ok;
	t_input			input;
	t_philo			*philos;
	pthread_t		*threads;
	pthread_t		watcher;
	pthread_mutex_t	*m_forks;
	pthread_mutex_t	m_print;
	pthread_mutex_t	m_time;
}	t_table;

int		parse_arg(const char *nptr);
int		init_table(int argc, char *argv[], t_table *table);
int		create_threads(t_table *table);
int		end_simulation(t_table *table);

size_t	get_time_ms(void);
void	ms_sleep(int t_ms);

void	print_msg(char *str, t_philo *philo);
void	print_error(char *err_msg, pthread_mutex_t *lock);

#endif