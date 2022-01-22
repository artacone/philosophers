/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 17:45:19 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/22 17:45:22 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>
#include <error.h>

/* TODO
 * 	Add specific error messages and errcodes
 * 	Check args are valid (numbers > 0) (maybe change atoi)
*/

static int	init_input(int argc, char *argv[], t_input *in)
{
	if (argc < 5 || argc > 6)
	{
		return (0);
	}
	in->n_philos = ft_atoi(argv[1]);
	in->time_to_die = ft_atoi(argv[2]);
	in->time_to_eat = ft_atoi(argv[3]);
	in->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 5)
	{
		in->n_to_eat = -1;
	}
	else
	{
		in->n_to_eat = ft_atoi(argv[5]);
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
		{
			return (0);
		}
	}
	table->mutex_forks = forks;
	if (pthread_mutex_init(&table->mutex_print, NULL) != 0)
	{
		return (0);
	}
	return (1);
}

static int	init_philos(t_table *table)
{
	int		i;
	t_philo	*philos;

	philos = malloc(table->input.n_philos * sizeof *philos);
	if (philos == NULL)
		return (0);
	i = 0;
	while (i < table->input.n_philos)
	{
		philos->left = &table->mutex_forks[i]; // FIXME
		philos->right = &table->mutex_forks[i + 1]; // FIXME
		philos->input = &table->input;
		++i;
	}
	table->philos = philos;
	return (1);
}

static int	init_threads(t_table *table)
{
	pthread_t	*threads;

	threads = malloc(table->input.n_philos * sizeof *threads);
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
	// TODO further init
	return (1);
}

int	main(int argc, char *argv[])
{
	t_table	table;

	// Initialize
	if (!init_table(argc, argv, &table))
	{
		return (ERROR_INPUT);
	}
	// Start threads
	if (!create_threads(&table))
	{
		return (2); // FIXME errcode
	}
	// Clear up (allocation -> free; destroy mutexes; ? join threads)

	return (0); // Consider returning status
}
