/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 19:50:22 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/22 19:50:23 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/philo.h"

static void	kill_philo(t_table *table, int id)
{
	sem_wait(table->sem_print);
	printf("%lu %d %s\n", get_time_ms() - table->t_start, id, MSG_DEATH);
	sem_post(table->sem_end);
}

static void	*watcher_routine(void *arg)
{
	t_philo	*philo;
	t_table	*table;
	int		id;

	philo = (t_philo *)arg;
	table = philo->table;
	id = philo->id;
	while (1)
	{
		sem_wait(philo->sem_time);
		if ((int)(get_time_ms() - philo->t_last_meal) > table->input.t_to_die)
			break ;
		sem_post(philo->sem_time);
		usleep(2000);
	}
	kill_philo(table, id);
	return (NULL);
}

static void	philo_routine(t_philo *philo)
{
	int	meals_eaten;

	meals_eaten = 0;
	sem_wait(philo->sem_time);
	philo->t_last_meal = get_time_ms();
	sem_post(philo->sem_time);
	while (1)
	{
		philo_eat(philo);
		++meals_eaten;
		check_fullness(philo->table, meals_eaten);
		philo_sleep(philo);
		philo_think(philo);
	}
}

static int	init_philo_semaphores(t_philo *philo)
{
	char	sem_name_time[256];
	int		i;
	int		n;

	n = philo->id;
	i = -1;
	while (SEM_NAME_TIME[++i])
		sem_name_time[i] = SEM_NAME_TIME[i];
	sem_name_time[i++] = '_';
	while (n)
	{
		sem_name_time[i++] = n % 10 + '0';
		n /= 10;
	}
	if (!semaphore_create(SEM_NAME_FORKS, 0, &philo->sem_forks)
		|| !semaphore_create(SEM_NAME_PRINT, 0, &philo->sem_print)
		|| !semaphore_create(SEM_NAME_START, 0, &philo->sem_start)
		|| !semaphore_create(SEM_NAME_FULL, 0, &philo->sem_fullness)
		|| !semaphore_create(SEM_NAME_END, 0, &philo->sem_end)
		|| !semaphore_create(SEM_NAME_TAKE, 0, &philo->sem_take)
		|| !semaphore_create(sem_name_time, 1, &philo->sem_time))
		return (0);
	return (1);
}

pid_t	start_philo(t_philo *philo)
{
	pthread_t	watcher;
	pid_t		pid;

	pid = fork();
	if (pid == 0)
	{
		philo->sem_end = SEM_FAILED;
		if (!init_philo_semaphores(philo))
		{
			if (philo->sem_end != SEM_FAILED)
				sem_post(philo->sem_end);
			exit(1);
		}
		sem_wait(philo->sem_start);
		philo->t_last_meal = get_time_ms();
		if (pthread_create(&watcher, NULL, watcher_routine, philo))
			exit(sem_post(philo->sem_end));
		pthread_detach(watcher);
		philo_routine(philo);
		exit(0);
	}
	return (pid);
}
