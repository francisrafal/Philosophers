/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:30:55 by frafal            #+#    #+#             */
/*   Updated: 2023/01/31 16:46:25 by frafal           ###   ########.fr       */
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

void	free_data(t_data *data)
{
	free_null(data->philo_threads);
	free_forks(data);
	pthread_mutex_destroy(&(data->print_mutex));
	pthread_mutex_destroy(&(data->tv1_mutex));
	pthread_mutex_destroy(&(data->alive_mutex));
	pthread_mutex_destroy(&(data->last_eaten_mutex));
	free_null(data->last_eaten);
	free_null(data);
}
