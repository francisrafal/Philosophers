/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 14:07:55 by frafal            #+#    #+#             */
/*   Updated: 2023/01/30 17:16:31 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_take_forks(t_data *data, t_philo *philo)
{
	int		left;
	int		right;
	int		id;

	left = philo->left;
	right = philo->right;
	id = philo->id;
	pthread_mutex_lock(data->forks + min(left, right));
	if (!still_alive(data))
		return (-1);
	if (philo->id % 2 == 0)
		usleep(400);
	print_msg(MSG_TAKE_FORK, data, id);
	pthread_mutex_lock(data->forks + max(left, right));
	if (!still_alive(data))
		return (-1);
	print_msg(MSG_TAKE_FORK, data, id);
	return (0);
}

void	philo_put_forks(t_data *data, t_philo *philo)
{
	int		left;
	int		right;

	left = philo->left;
	right = philo->right;
	pthread_mutex_unlock(data->forks + max(left, right));
	pthread_mutex_unlock(data->forks + min(left, right));
}

void	philo_eat(t_data *data, t_philo *philo)
{
	print_msg(MSG_EATING, data, philo->id);
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
	return (0);
}