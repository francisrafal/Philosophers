/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 14:07:55 by frafal            #+#    #+#             */
/*   Updated: 2023/01/31 19:48:18 by frafal           ###   ########.fr       */
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
	if (data->num == 1)
	{
		pthread_mutex_lock(data->forks + left);
		print_msg(MSG_TAKE_FORK, data, id);
		pthread_mutex_unlock(data->forks + left);
		return (-1);
	}
	if (philo->id % 2 == 0)
	{	
		if (philo_take_forks_even(data, philo) == -1)
			return (-1);
	}
	else
	{	
		if (philo_take_forks_odd(data, philo) == -1)
			return (-1);
	}
	return (0);
}

int	philo_take_forks_odd(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(data->forks + philo->right);
	if (!still_alive(data))
	{	
		pthread_mutex_unlock(data->forks + philo->right);
		return (-1);
	}
	print_msg(MSG_TAKE_FORK, data, philo->id);
	pthread_mutex_lock(data->forks + philo->left);
	if (!still_alive(data))
	{	
		pthread_mutex_unlock(data->forks + philo->right);
		pthread_mutex_unlock(data->forks + philo->left);
		return (-1);
	}
	print_msg(MSG_TAKE_FORK, data, philo->id);
	return (0);
}

int	philo_take_forks_even(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(data->forks + philo->left);
	if (!still_alive(data))
	{	
		pthread_mutex_unlock(data->forks + philo->left);
		return (-1);
	}
	print_msg(MSG_TAKE_FORK, data, philo->id);
	pthread_mutex_lock(data->forks + philo->right);
	if (!still_alive(data))
	{	
		pthread_mutex_unlock(data->forks + philo->left);
		pthread_mutex_unlock(data->forks + philo->right);
		return (-1);
	}
	print_msg(MSG_TAKE_FORK, data, philo->id);
	return (0);
}

void	philo_put_forks(t_data *data, t_philo *philo)
{
	int		left;
	int		right;

	left = philo->left;
	right = philo->right;
	pthread_mutex_unlock(data->forks + left);
	pthread_mutex_unlock(data->forks + right);
}
