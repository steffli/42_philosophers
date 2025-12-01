/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stliu <stliu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:35:49 by stliu             #+#    #+#             */
/*   Updated: 2025/12/01 12:35:49 by stliu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//n_philo time_to_die time_to_eat time_to_sleep [n_times_eat]
static int		ft_isnum(char *str)
{
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (0);
		str++;
	}
	return (1);
}
int		validate(int n, char **input)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (!ft_isnum(input[i]))
			return(0);
		i++;
	}
	return (1);
}

int	ft_atoi(char *str)
{
	int	num;

	num = 0;
	while (*str)
	{
		num = num * 10 + (*str - '0');
		str++;
	}
	return (num);
}

void	error(void)
{
    printf("%s", "Please provide correct input.\n"
        "number_of_philosophers\n"
        "time_to_die\n"
        "time_to_eat\n"
        "time_to_sleep\n"
        "[number_of_meals_per_philosopher]\n");
}