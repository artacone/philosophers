/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 17:24:22 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/23 17:24:24 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	init_input(int argc, char *argv[], t_input *in)
{
	if (argc != 5 && argc != 6)
	{
		printf("Error: wrong number of args.\n");
		return (0);
	}
	in->n_philos = parse_arg(argv[1]);
	in->t_to_die = parse_arg(argv[2]);
	in->t_to_eat = parse_arg(argv[3]);
	in->t_to_sleep = parse_arg(argv[4]);
	if (argc == 5)
		in->n_to_eat = -1;
	else
		in->n_to_eat = parse_arg(argv[5]);
	if (in->n_philos * in->t_to_die * in->t_to_eat
		* in->t_to_sleep * in->n_to_eat == 0)
	{
		printf("Error: wrong arguments.\n");
		return (0);
	}
	return (1);
}

int	semaphore_create(const char *name, int value, sem_t **sem)
{
	if (value == 0)
	{
		*sem = sem_open(name, 0);
	}
	else
	{
		sem_unlink(name);
		*sem = sem_open(name, O_CREAT | O_EXCL, 0700, value);
	}
	return (*sem != SEM_FAILED);
}

static int	init_semaphores(t_table *table)
{
	int	n;

	table->sem_forks = SEM_FAILED; // FIXME ???
	table->sem_print = SEM_FAILED;
	table->sem_start = SEM_FAILED;
	table->sem_fullness = SEM_FAILED;
	n = table->input.n_philos;
	if (!semaphore_create(SEM_NAME_FORKS, n, &table->sem_forks)
		|| !semaphore_create(SEM_NAME_PRINT, 1, &table->sem_print)
		|| !semaphore_create(SEM_NAME_START, 1, &table->sem_start)
		|| !semaphore_create(SEM_NAME_FULL, n, &table->sem_fullness)
		|| !semaphore_create(SEM_NAME_END, 1, &table->sem_end)
		|| !semaphore_create(SEM_NAME_GRAB, 1, &table->sem_grab))
		return (0);
	return (1);
}

static int	init_philos(int n, t_table *table, pid_t *pids)
{
	int		i;
	size_t	t_start;
	t_philo	philo;

	t_start = get_time_ms();
	i = -1;
	while (++i < n)
		sem_wait(table->sem_start);
	i = -1;
	while (++i < n)
	{
		philo.table = table;
		philo.id = i + 1;
		philo.t_start = t_start;
		pids[i] = start_philo(&philo);
		if (pids[i] == -1)
			return (0);
	}
	i = -1;
	while (++i < n)
		sem_post(table->sem_start);
	return (1);
}

int	init_table(int argc, char *argv[], t_table *table, pid_t **pids)
{
	if (!init_input(argc, argv, &table->input))
		return (0);
	if (!init_semaphores(table))
		return (0);
	*pids = malloc(table->input.n_philos * sizeof **pids);
	if (*pids == NULL)
		return (0);
	if (!init_philos(table->input.n_philos, table, *pids))
		return (0);
	table->ok = 1;
	table->n_full = 0;
	table->t_start = get_time_ms();
	return (1);
}
