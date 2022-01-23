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
	int	n_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	n_to_eat;
}	t_input;

typedef struct s_philo {
	int				id;
	int				is_alive;
	int				meals_left;
	size_t			last_meal_time;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	t_input			*input;
}	t_philo;

typedef struct s_table {
	t_input			input;
	int				hungry; // init
	t_philo			*philos;
	pthread_t		*threads;
	pthread_t		watcher;
	pthread_mutex_t	*mutex_forks;
	pthread_mutex_t	mutex_print;
}	t_table;

int	ft_atoi(const char *nptr);
int	init_table(int argc, char *argv[], t_table *table);
int	create_threads(t_table *table);
int	end_simulation(t_table *table);

size_t	get_time_ms(void);

void	print_msg(char *str, t_philo * philo, pthread_mutex_t *mutex_print);

#endif