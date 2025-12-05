/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stliu <stliu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 09:19:39 by stliu             #+#    #+#             */
/*   Updated: 2025/12/05 09:19:39 by stliu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_table	*table;
}	t_philo;

typedef struct s_table
{
	int				n_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_meals;
	int				dead;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
	t_philo			*philos;
}	t_table;

// Input validation and parsing
int		validate(int n, char **input);
void	error(void);
int		ft_atoi(char *str);

// Init
int		init_table(t_table *table, char **argv);

// Philo routine
void	*philo_routine(void *arg);
void	print_status(t_philo *philo, char *state);
void	start_sim(t_table *table);

// Custom time utils
long	get_time(void);
void	ft_usleep(long time);
void	think_time(t_philo *philo);

// errors
void	cleanup(t_table *table);

#endif