/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:36:17 by frafal            #+#    #+#             */
/*   Updated: 2023/01/30 15:38:14 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	gettimeofday_safe(t_data *data)
{
	pthread_mutex_lock(&(data->tv1_mutex));
	gettimeofday(&(data->tv1), NULL);
	pthread_mutex_unlock(&(data->tv1_mutex));
}

long	time_diff(struct timeval a, struct timeval b)
{
	return ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_usec - b.tv_usec) / 1000);
}

long	get_timestamp(t_data *data)
{
	gettimeofday_safe(data);
	return (time_diff(data->tv1, data->tv0));
}

struct timeval	get_last_eaten(t_data *data, int id)
{
	struct timeval	last_eaten;

	pthread_mutex_lock(&(data->last_eaten_mutex));
	last_eaten = data->last_eaten[id];
	pthread_mutex_unlock(&(data->last_eaten_mutex));
	return (last_eaten);
}

void	set_last_eaten(t_data *data)
{
	int	id;

	id = 1;
	pthread_mutex_lock(&(data->last_eaten_mutex));
	while (id <= data->num)
		data->last_eaten[id++] = data->tv0;
	pthread_mutex_unlock(&(data->last_eaten_mutex));
}
