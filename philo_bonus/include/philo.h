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

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <semaphore.h>

# define MSG_FORK	"has taken a fork"
# define MSG_EAT	"is eating"
# define MSG_SLEEP	"is sleeping"
# define MSG_THINK	"is thinking"
# define MSG_DEATH	"died"

# define SEM_NAME_FORKS	"philo_forks"
# define SEM_NAME_PRINT	"philo_print"
# define SEM_NAME_END	"philo_end"
# define SEM_NAME_FULL	"philo_fullness"
# define SEM_NAME_START "philo_start"
# define SEM_NAME_TAKE	"philo_take"
# define SEM_NAME_TIME	"philo_time"

typedef struct s_input {
	int		n_philos;
	int		t_to_die;
	int		t_to_eat;
	int		t_to_sleep;
	int		n_to_eat;
}	t_input;

typedef struct s_table {
	t_input	input;
	size_t	t_start;
	sem_t	*sem_forks;
	sem_t	*sem_fullness;
	sem_t	*sem_print;
	sem_t	*sem_start;
	sem_t	*sem_end;
	sem_t	*sem_take;
}	t_table;

typedef struct s_philo {
	int		id;
	size_t	t_last_meal;
	size_t	t_start;
	sem_t	*sem_forks;
	sem_t	*sem_fullness;
	sem_t	*sem_print;
	sem_t	*sem_start;
	sem_t	*sem_end;
	sem_t	*sem_take;
	sem_t	*sem_time;
	t_table	*table;
}	t_philo;

int		parse_arg(const char *nptr);
int		init_table(int argc, char *argv[], t_table *table, pid_t **pids);
int		semaphore_create(const char *name, int value, sem_t **sem);
pid_t	start_philo(t_philo *philo);
int		end_simulation(t_table *table, pid_t *philos);
void	destroy_philos(int n, pid_t *philos);

size_t	get_time_ms(void);
void	ms_sleep(int t_ms);

void	print_msg(char *str, t_philo *philo);

void	philo_take_fork(t_philo *philo);
void	check_fullness(t_table *table, int meals_eaten);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

#endif