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

#include "../include/main.h"

/* TODO
 * 	Add specific error messages and errcodes
 * 	Check args are valid (numbers > 0) (maybe change atoi)
*/

static int	init_input(int argc, char *argv[], t_input *in)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of args.\n");
		return (0);
	}
	in->n_philos = parse_arg(argv[1]);
	in->t_to_die = parse_arg(argv[2]);
	in->t_to_eat = parse_arg(argv[3]);
	in->t_to_sleep = parse_arg(argv[4]);
	if (argc == 5)
	{
		in->n_to_eat = -1;
		in->hungry = -1;
	}
	else
	{
		in->n_to_eat = parse_arg(argv[5]);
		in->hungry = in->n_philos;
	}
	if (in->n_philos * in->t_to_die * in->t_to_eat
		* in->t_to_sleep * in->n_to_eat == 0)
	{
		printf("Error: wrong arguments.\n");
		return (0);
	}
	return (1);
}

static int	init_mutexes(t_table *table)
{
	int				n;
	pthread_mutex_t	*forks;

	n = table->input.n_philos;
	forks = malloc(n * sizeof(*forks));
	if (forks == NULL)
		return (0);
	while (n > 0)
	{
		if (pthread_mutex_init(&forks[--n], NULL) != 0)
			return (0);
	}
	table->m_forks = forks;
	if (pthread_mutex_init(&table->m_print, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&table->m_time, NULL) != 0)
		return (0);
	return (1);
}

static int	init_philos(t_table *table)
{
	int		i;
	int		n;
	t_philo	*philos;

	n = table->input.n_philos;
	philos = malloc(n * sizeof(*philos));
	if (philos == NULL)
		return (0);
	i = 0;
	while (i < n)
	{
		philos[i].id = i + 1;
		philos[i].is_alive = 1;
		philos[i].first = &table->m_forks[i];
		philos[i].second = &table->m_forks[(i + 1) % n];
		philos[i].print = &table->m_print;
		philos[i].time = &table->m_time;
		philos[i].input = &table->input;
		philos[i].meals_left = philos->input->n_to_eat;
		++i;
	}
//	philos[n - 1].first = &table->m_forks[0];
//	philos[n - 1].second = &table->m_forks[n - 1];
	table->philos = philos;
	return (1);
}

static int	init_threads(t_table *table)
{
	pthread_t	*threads;

	threads = malloc(table->input.n_philos * sizeof(*threads));
	if (threads == NULL)
		return (0);
	table->threads = threads;
	return (1);
}

int	init_table(int argc, char *argv[], t_table *table)
{
	if (!init_input(argc, argv, &table->input))
		return (0);
	if (!init_mutexes(table))
		return (0);
	if (!init_philos(table))
		return (0);
	if (!init_threads(table))
		return (0);
	table->input.t_start = get_time_ms();
	return (1);
}
