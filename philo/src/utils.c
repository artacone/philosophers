/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 18:49:29 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/22 18:49:32 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isspace(int c)
{
	return (c == ' ' || ('\t' <= c && c <= '\r'));
}

int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	n;

	n = 0;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}
	else
	{
		sign = 1;
		if (*nptr == '+')
			nptr++;
	}
	while (ft_isdigit(*nptr))
		n = n * 10 + (*nptr++ - '0');
	return (sign * n);
}
