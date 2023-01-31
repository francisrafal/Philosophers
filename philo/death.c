/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:32:48 by frafal            #+#    #+#             */
/*   Updated: 2023/01/31 17:24:19 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	still_alive(t_data *data)
{
	pthread_mutex_lock(&(data->alive_mutex));
	if (!data->all_alive)
	{
		pthread_mutex_unlock(&(data->alive_mutex));
		return (0);
	}
	pthread_mutex_unlock(&(data->alive_mutex));
	return (1);
}

void	philo_dead(t_data *data, int id)
{
	print_msg(MSG_DIED, data, id);
	pthread_mutex_lock(&(data->alive_mutex));
	data->all_alive = 0;
	pthread_mutex_unlock(&(data->alive_mutex));
}

void	*check_deaths(void *ptr)
{
	t_data			*data;
	int				id;
	struct timeval	now;
	struct timeval	last_eaten;

	data = (t_data *)ptr;
	while (still_alive(data))
	{
		id = 1;
		while (id <= data->num)
		{
			now = gettimeofday_safe(data);
			last_eaten = get_last_eaten(data, id);
			if (time_diff(now, last_eaten) > data->ttd)
			{
				philo_dead(data, id);
				break ;
			}
			id++;
		}
	}
	return (NULL);
}
