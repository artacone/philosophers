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
	if (pthread_mutex_init(&table->mutex_print, NULL) != 0)
	{
		return (0);
	}
	return (1);
}

static int	init_threads(t_table *table)
{
	table->philos = malloc(table->input.n_philos * sizeof(*(table->philos)));
	if (table->philos == NULL)
		return (0);
	return (1);
}

int	init_table(int argc, char *argv[], t_table *table)
{
	if (!init_input(argc, argv, &table->input))
		return (0);
	if (!init_mutexes(table))
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
	// Clear up

	return (0); // Consider returning status
}
