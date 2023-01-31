/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:30:55 by frafal            #+#    #+#             */
/*   Updated: 2023/01/31 17:38:53 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_null(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

int	free_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num)
	{
		if (pthread_mutex_destroy(data->forks + i++))
			return (-1);
	}
	free_null(data->forks);
	return (0);
}

int	free_data(t_data *data)
{
	free_null(data->philo_threads);
	if (free_forks(data) == -1)
		return (-1);
	if (pthread_mutex_destroy(&(data->print_mutex)))
		return (-1);
	if (pthread_mutex_destroy(&(data->tv1_mutex)))
		return (-1);
	if (pthread_mutex_destroy(&(data->alive_mutex)))
		return (-1);
	if (pthread_mutex_destroy(&(data->last_eaten_mutex)))
		return (-1);
	free_null(data->last_eaten);
	free_null(data);
	return (0);
}
