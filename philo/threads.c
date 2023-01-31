/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:40:34 by frafal            #+#    #+#             */
/*   Updated: 2023/01/31 19:28:03 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(int msg, t_data *data, int id)
{
	long	time;

	time = get_timestamp(data);
	pthread_mutex_lock(&(data->print_mutex));
	if (msg == MSG_TAKE_FORK)
		printf("%ld %d has taken a fork\n", time, id);
	else if (msg == MSG_EATING)
		printf("%ld %d is eating\n", time, id);
	else if (msg == MSG_SLEEPING)
		printf("%ld %d is sleeping\n", time, id);
	else if (msg == MSG_THINKING)
		printf("%ld %d is thinking\n", time, id);
	else if (msg == MSG_DIED)
		printf("%ld %d died\n", time, id);
	pthread_mutex_unlock(&(data->print_mutex));
}

void	*philosopher_thread(void *ptr)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)ptr;
	data = philo->data;
	while (still_alive(data))
	{
		if (philo_take_forks(data, philo) == -1)
			break ;
		philo_eat(data, philo);
		philo_put_forks(data, philo);
		if (philo_sleep(data, philo) == -1)
			break ;
		if (philo_think(data, philo) == -1)
			break ;
	}
	return (NULL);
}

void	start_philosophers(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->num)
	{
		philos[i].left = i;
		philos[i].right = (i + 1) % data->num;
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].times_eaten = 0;
		pthread_create(&(data->philo_threads[i]), NULL,
			philosopher_thread, philos + i);
		i++;
	}
}

void	join_all_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num)
		pthread_join(data->philo_threads[i++], NULL);
	pthread_join(data->death_thread, NULL);
}
