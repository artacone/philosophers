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
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	t_input			*input;
}	t_philo;

typedef struct s_table {
	t_input			input;
	t_philo 		*philos;
	pthread_t		*threads;
	pthread_t		watcher;
	pthread_mutex_t	*mutex_forks;
	pthread_mutex_t	mutex_print;
}	t_table;

int	ft_atoi(const char *nptr);
int	create_threads(t_table *table);

#endif