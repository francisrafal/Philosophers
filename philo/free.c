/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:30:55 by frafal            #+#    #+#             */
/*   Updated: 2023/01/30 15:31:39 by frafal           ###   ########.fr       */
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

void	free_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num)
		pthread_mutex_destroy(data->forks + i++);
	free_null(data->forks);
}

void	free_waiters(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_waiters)
		pthread_mutex_destroy(data->waiters + i++);
	free_null(data->waiters);
}

void	free_data(t_data *data)
{
	free_null(data->philo_threads);
	free_forks(data);
	free_waiters(data);
	pthread_mutex_destroy(&(data->print_mutex));
	pthread_mutex_destroy(&(data->tv1_mutex));
	pthread_mutex_destroy(&(data->alive_mutex));
	pthread_mutex_destroy(&(data->last_eaten_mutex));
	pthread_mutex_destroy(&(data->queue_mutex));
	free_null(data->queue);
	free_null(data->last_eaten);
	free_null(data);
}
