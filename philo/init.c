/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stliu <stliu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:13:13 by stliu             #+#    #+#             */
/*   Updated: 2025/12/01 14:15:55 by stliu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_table(t_table *table, char **argv)
{
	table->n_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		table->n_meals = ft_atoi(argv[5]);
	table->dead = 0;
	table->start_time = get_time();
	table->forks = malloc(sizeof(pthread_mutex_t) * table->n_philos);
	if (!table->forks)
		return (0);
	table->philos = malloc(sizeof(t_philo) * table->n_philos);
	if (!table->philos)
		return (free(table->forks), 0);
	return (1);
}