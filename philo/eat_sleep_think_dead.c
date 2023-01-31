/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think_dead.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 14:07:55 by frafal            #+#    #+#             */
/*   Updated: 2023/01/31 19:31:45 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_data *data, t_philo *philo)
{
	print_msg(MSG_EATING, data, philo->id);
	pthread_mutex_lock(&(data->philos_mutex));
	philo->times_eaten++;
	pthread_mutex_unlock(&(data->philos_mutex));
	pthread_mutex_lock(&(data->last_eaten_mutex));
	gettimeofday(data->last_eaten + philo->id, NULL);
	pthread_mutex_unlock(&(data->last_eaten_mutex));
	usleep(data->tte * 1000);
}

int	philo_sleep(t_data *data, t_philo *philo)
{
	if (!still_alive(data))
		return (-1);
	print_msg(MSG_SLEEPING, data, philo->id);
	usleep(data->tts * 1000);
	return (0);
}

int	philo_think(t_data *data, t_philo *philo)
{
	if (!still_alive(data))
		return (-1);
	print_msg(MSG_THINKING, data, philo->id);
	usleep(400);
	return (0);
}

void	philo_dead(t_data *data, int id)
{
	print_msg(MSG_DIED, data, id);
	pthread_mutex_lock(&(data->alive_mutex));
	data->all_alive = 0;
	pthread_mutex_unlock(&(data->alive_mutex));
}
