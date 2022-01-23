/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 18:21:36 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/23 18:21:38 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/main.h"

void	print(char *str, pthread_mutex_t *mutex_print)
{
	pthread_mutex_lock(mutex_print);
	printf("msg");
	pthread_mutex_unlock(mutex_print);
}
