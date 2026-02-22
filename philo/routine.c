/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stliu <stliu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:39:41 by stliu             #+#    #+#             */
/*   Updated: 2025/12/04 15:39:41 by stliu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	get_forks_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (philo->left_fork < philo->right_fork)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
}

static void	take_forks(t_philo *philo, pthread_mutex_t *first,
		pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	print_status(philo, "has taken a fork");
	if (philo->table->n_philos == 1)
	{
		pthread_mutex_unlock(first);
		return ;
	}
	pthread_mutex_lock(second);
	print_status(philo, "has taken a fork");
}

static void	philo_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	get_forks_order(philo, &first, &second);
	take_forks(philo, first, second);
	if (philo->table->n_philos == 1)
		return ;
	pthread_mutex_lock(&philo->table->death_lock);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->table->death_lock);
	print_status(philo, "is eating");
	if (ft_usleep(philo->table->time_to_eat, philo->table))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return ;
	}
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

static int	should_stop(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(&philo->table->death_lock);
	result = 0;
	if (philo->table->dead)
		result = 1;
	else if (philo->table->n_meals != -1
		&& philo->meals_eaten >= philo->table->n_meals)
		result = 1;
	pthread_mutex_unlock(&philo->table->death_lock);
	return (result);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->n_philos == 1)
		return (one_philo_routine(philo));
	if (philo->id % 2 == 0)
		ft_usleep(1, philo->table);
	while (1)
	{
		if (should_stop(philo))
			break ;
		philo_eat(philo);
		if (should_stop(philo))
			break ;
		print_status(philo, "is sleeping");
		if (ft_usleep(philo->table->time_to_sleep, philo->table))
			break ;
		print_status(philo, "is thinking");
		if (think_time(philo))
			break ;
	}
	return (NULL);
}
